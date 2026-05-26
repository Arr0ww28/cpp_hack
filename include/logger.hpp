/**
 * @file logger.hpp
 * @brief Event Logger Module — Thread-Safe Logging Infrastructure
 *
 * Provides timestamped, severity-tagged logging to both file and in-memory
 * history. Includes a thread-safe queue template for async log processing,
 * and global LOG_ macros for function-level debug tracing.
 *
 * C++ Concepts Demonstrated:
 *   - Templates (ThreadSafeQueue<T>)
 *   - RAII (std::ofstream file handle)
 *   - Move Semantics (LogEntry move ctor/assignment)
 *   - Operator Overloading (operator<< for LogEntry)
 *   - Lambdas (searchEvents predicate)
 *   - STL Algorithms (std::copy_if for filtering)
 *   - File Streams (std::ofstream for log writing)
 *   - Enum Class (LogLevel)
 *   - Condition Variables (ThreadSafeQueue wait/notify)
 *   - Mutex / lock_guard (thread-safe queue and history)
 */

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

// =============================================================================
// LogLevel Enumeration (independent of AlertSeverity)
// =============================================================================

/**
 * @enum LogLevel
 * @brief Severity levels for log entries.
 *        Separate from AlertSeverity to keep logger module standalone.
 */
enum class LogLevel {
    DEBUG,      ///< Function-level tracing, verbose
    INFO,       ///< Informational messages (startup, config)
    WARNING,    ///< Non-critical issues (low battery, overspeed)
    CRITICAL    ///< Critical conditions (engine overheat, door open at speed)
};

// =============================================================================
// LogEntry — A single log record
// =============================================================================

/**
 * @struct LogEntry
 * @brief Represents a single timestamped log event.
 *
 * Supports move semantics for efficient queue transfers.
 * Supports copy for search result returns.
 */
struct LogEntry {
    std::string timestamp;    ///< ISO-8601 formatted timestamp
    LogLevel    level;        ///< Severity level
    std::string source;       ///< Module/class that generated the log
    std::string message;      ///< Human-readable log message

    /**
     * @brief Construct a new LogEntry with auto-generated timestamp.
     */
    LogEntry(LogLevel lvl, std::string src, std::string msg);

    /**
     * @brief Construct a LogEntry with explicit timestamp (for deserialization).
     */
    LogEntry(std::string ts, LogLevel lvl, std::string src, std::string msg);

    /// Default copy
    LogEntry(const LogEntry&) = default;
    LogEntry& operator=(const LogEntry&) = default;

    /// Move-optimized
    LogEntry(LogEntry&& other) noexcept;
    LogEntry& operator=(LogEntry&& other) noexcept;

    /// Default constructor (for queue operations)
    LogEntry() : level(LogLevel::DEBUG) {}

    /**
     * @brief Convert LogLevel to a fixed-width string tag.
     */
    static std::string levelToString(LogLevel lvl);

    /**
     * @brief Stream insertion for formatted output.
     *        Format: [timestamp] [LEVEL] [source] message
     */
    friend std::ostream& operator<<(std::ostream& os, const LogEntry& entry);
};

// =============================================================================
// ThreadSafeQueue<T> — Generic Lock-Free Producer/Consumer Queue
// =============================================================================

/**
 * @class ThreadSafeQueue
 * @brief A thread-safe FIFO queue using mutex + condition_variable.
 *
 * Used by EventLogger to decouple log producers (any thread) from the
 * consumer (logger thread that writes to file).
 *
 * @tparam T  Element type (must be move-constructible).
 */
template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T>           queue_;
    mutable std::mutex      mtx_;
    std::condition_variable cv_;

public:
    /**
     * @brief Push an item into the queue (move semantics).
     *        Notifies one waiting consumer.
     */
    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(item));
        }
        cv_.notify_one();
    }

    /**
     * @brief Non-blocking pop. Returns true if an item was dequeued.
     */
    bool tryPop(T& item) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (queue_.empty()) {
            return false;
        }
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    /**
     * @brief Blocking pop. Waits until an item is available or predicate fails.
     * @param item     Output: the dequeued item.
     * @param running  Atomic flag — returns false if set to false (shutdown).
     */
    bool waitAndPop(T& item, const std::atomic<bool>& running) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [&]() { return !queue_.empty() || !running.load(); });
        if (!running.load() && queue_.empty()) {
            return false;
        }
        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    /**
     * @brief Check if the queue is empty.
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.empty();
    }

    /**
     * @brief Get the current queue size.
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return queue_.size();
    }

    /**
     * @brief Wake all waiting consumers (used during shutdown).
     */
    void notifyAll() {
        cv_.notify_all();
    }
};

// =============================================================================
// EventLogger — File-Backed Async Logger
// =============================================================================

/**
 * @class EventLogger
 * @brief Thread-safe event logger with async file writing.
 *
 * Log events are enqueued via logEvent() from any thread. A dedicated
 * logger thread calls processPendingEvents() to drain the queue and
 * write entries to the log file and in-memory history.
 *
 * Supports lambda-based search/filter over the in-memory history
 * using STL algorithms (std::copy_if).
 */
class EventLogger {
private:
    std::string                  logFilePath_;     ///< Path to log file
    std::ofstream                logFile_;         ///< RAII file handle
    std::vector<LogEntry>        eventHistory_;    ///< In-memory history
    ThreadSafeQueue<LogEntry>    pendingQueue_;    ///< Async write queue
    mutable std::mutex           historyMtx_;      ///< Guards eventHistory_
    bool                         fileOpen_;        ///< Track file state

public:
    /**
     * @brief Construct and open the log file for appending.
     * @param filePath  Path to the log file (e.g., "logs/vehicle_logs.txt").
     * @throws Prints warning if file cannot be opened (does not throw).
     */
    explicit EventLogger(const std::string& filePath);

    /**
     * @brief Destructor — flushes pending events and closes file.
     */
    ~EventLogger();

    // Non-copyable, non-movable (owns file handle and threads reference it)
    EventLogger(const EventLogger&) = delete;
    EventLogger& operator=(const EventLogger&) = delete;

    /**
     * @brief Enqueue a log event for async writing.
     *        Can be called safely from any thread.
     * @param entry  The LogEntry to enqueue (moved into queue).
     */
    void logEvent(LogEntry entry);

    /**
     * @brief Convenience: create and enqueue a LogEntry in one call.
     */
    void log(LogLevel level, const std::string& source, const std::string& message);

    /**
     * @brief Process all pending events in the queue.
     *        Called by the logger thread in a loop. Writes each entry
     *        to the file and appends to in-memory history.
     */
    void processPendingEvents();

    /**
     * @brief Flush any remaining pending events (for shutdown).
     */
    void flushAll();

    /**
     * @brief Search event history using a lambda predicate.
     *        Uses std::copy_if internally.
     * @param predicate  Lambda that returns true for matching entries.
     * @return Vector of matching LogEntry objects.
     */
    std::vector<LogEntry> searchEvents(
        std::function<bool(const LogEntry&)> predicate) const;

    /**
     * @brief Get the most recent N events from history.
     * @param count  Number of recent events to return.
     */
    std::vector<LogEntry> getRecentEvents(size_t count) const;

    /**
     * @brief Get total number of events logged.
     */
    size_t getTotalEventCount() const;

    /**
     * @brief Get reference to the pending queue (for shutdown notification).
     */
    ThreadSafeQueue<LogEntry>& getPendingQueue() { return pendingQueue_; }
};

// =============================================================================
// Global Logger Access
// =============================================================================

/**
 * @brief Set the global logger instance. Must be called before any LOG_ calls.
 *        Does NOT take ownership — caller must ensure logger outlives all users.
 */
void setGlobalLogger(EventLogger* logger);

/**
 * @brief Get the global logger instance. Returns nullptr if not set.
 */
EventLogger* getGlobalLogger();

// =============================================================================
// Convenience Logging Functions
// =============================================================================

/**
 * @brief Log a DEBUG-level message through the global logger.
 *        No-op if global logger is not set.
 */
void LOG_DEBUG(const std::string& source, const std::string& message);

/**
 * @brief Log an INFO-level message through the global logger.
 */
void LOG_INFO(const std::string& source, const std::string& message);

/**
 * @brief Log a WARNING-level message through the global logger.
 */
void LOG_WARNING(const std::string& source, const std::string& message);

/**
 * @brief Log a CRITICAL-level message through the global logger.
 */
void LOG_CRITICAL(const std::string& source, const std::string& message);

#endif // LOGGER_HPP
