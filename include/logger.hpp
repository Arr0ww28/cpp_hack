#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <functional>
#include <atomic>
#include <algorithm>

// Severity levels for log entries
enum class LogLevel {
    DEBUG, INFO, WARNING, CRITICAL
};

// Represents a single timestamped log event
struct LogEntry {
    std::string timestamp;
    LogLevel    level;
    std::string source;
    std::string message;

    LogEntry(LogLevel lvl, std::string src, std::string msg);
    LogEntry(std::string ts, LogLevel lvl, std::string src, std::string msg);
    LogEntry(const LogEntry&) = default;
    LogEntry& operator=(const LogEntry&) = default;
    LogEntry(LogEntry&& other) noexcept;
    LogEntry& operator=(LogEntry&& other) noexcept;
    LogEntry() : level(LogLevel::DEBUG) {}

    static std::string levelToString(LogLevel lvl);
    friend std::ostream& operator<<(std::ostream& os, const LogEntry& entry);
};

// Thread-safe FIFO queue
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T>           queue_;
    mutable std::mutex      mtx_;
    std::condition_variable cv_;

public:
    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(item));
        }
        cv_.notify_one();
    }

    bool tryPop(T& item) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool waitAndPop(T& item, const std::atomic<bool>& running) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]() { return !queue_.empty() || !running.load(); });
        if (!running.load() && queue_.empty()) return false;
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.size();
    }

    void notifyAll() { cv_.notify_all(); }
};

// Thread-safe event logger
class EventLogger {
private:
    std::string                  logFilePath_;
    std::ofstream                logFile_;
    std::vector<LogEntry>        eventHistory_;
    ThreadSafeQueue<LogEntry>    pendingQueue_;
    mutable std::mutex           historyMtx_;
    bool                         fileOpen_;

public:
    explicit EventLogger(const std::string& filePath);
    ~EventLogger();

    EventLogger(const EventLogger&) = delete;
    EventLogger& operator=(const EventLogger&) = delete;

    void logEvent(LogEntry entry);
    void log(LogLevel level, const std::string& source, const std::string& message);
    void processPendingEvents();
    void flushAll();
    std::vector<LogEntry> searchEvents(std::function<bool(const LogEntry&)> predicate) const;
    std::vector<LogEntry> getRecentEvents(size_t count) const;
    size_t getTotalEventCount() const;
    ThreadSafeQueue<LogEntry>& getPendingQueue() { return pendingQueue_; }
};

void setGlobalLogger(EventLogger* logger);
EventLogger* getGlobalLogger();

void log_impl(LogLevel level, const std::string& source, const std::string& file, const std::string& func, const std::string& message);

#define LOG_DEBUG(source, message)    log_impl(LogLevel::DEBUG, source, __FILE__, __func__, message)
#define LOG_INFO(source, message)     log_impl(LogLevel::INFO, source, __FILE__, __func__, message)
#define LOG_WARNING(source, message)  log_impl(LogLevel::WARNING, source, __FILE__, __func__, message)
#define LOG_CRITICAL(source, message) log_impl(LogLevel::CRITICAL, source, __FILE__, __func__, message)

#endif // LOGGER_HPP
