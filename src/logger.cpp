#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

// Generate a timestamp string
static std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

LogEntry::LogEntry(LogLevel lvl, std::string src, std::string msg)
    : timestamp(getCurrentTimestamp()), level(lvl), source(std::move(src)), message(std::move(msg)) {}

LogEntry::LogEntry(std::string ts, LogLevel lvl, std::string src, std::string msg)
    : timestamp(std::move(ts)), level(lvl), source(std::move(src)), message(std::move(msg)) {}

LogEntry::LogEntry(LogEntry&& other) noexcept
    : timestamp(std::move(other.timestamp)), level(other.level), source(std::move(other.source)), message(std::move(other.message)) {}

LogEntry& LogEntry::operator=(LogEntry&& other) noexcept {
    if (this != &other) {
        timestamp = std::move(other.timestamp); level = other.level;
        source = std::move(other.source); message = std::move(other.message);
    }
    return *this;
}

std::string LogEntry::levelToString(LogLevel lvl) {
    switch (lvl) {
        case LogLevel::DEBUG:    return "DEBUG   ";
        case LogLevel::INFO:     return "INFO    ";
        case LogLevel::WARNING:  return "WARNING ";
        case LogLevel::CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN ";
}

std::ostream& operator<<(std::ostream& os, const LogEntry& entry) {
    os << "[" << entry.timestamp << "] [" << LogEntry::levelToString(entry.level)
       << "] [" << std::left << std::setw(28) << entry.source << "] " << entry.message;
    return os;
}

EventLogger::EventLogger(const std::string& filePath)
    : logFilePath_(filePath), fileOpen_(false)
{
    try {
        logFile_.open(filePath, std::ios::out | std::ios::app);
        if (logFile_.is_open()) {
            fileOpen_ = true;
            logFile_ << "\n================================================================\n"
                     << "  Vehicle Health Monitor — Log Session Started\n"
                     << "  Timestamp: " << getCurrentTimestamp() << "\n"
                     << "================================================================\n" << std::flush;
        } else {
            std::cerr << "[LOGGER] WARNING: Cannot open log file: " << filePath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[LOGGER] ERROR: Exception opening log file: " << e.what() << std::endl;
        fileOpen_ = false;
    }
}

EventLogger::~EventLogger() {
    flushAll();
    if (fileOpen_ && logFile_.is_open()) {
        logFile_ << "================================================================\n"
                 << "  Log Session Ended: " << getCurrentTimestamp() << "\n"
                 << "  Total Events Logged: " << eventHistory_.size() << "\n"
                 << "================================================================\n" << std::flush;
        logFile_.close();
    }
}

void EventLogger::logEvent(LogEntry entry) {
    pendingQueue_.push(std::move(entry));
}

void EventLogger::log(LogLevel level, const std::string& source, const std::string& message) {
    logEvent(LogEntry(level, source, message));
}

void EventLogger::processPendingEvents() {
    LogEntry entry;
    while (pendingQueue_.tryPop(entry)) {
        if (fileOpen_ && logFile_.is_open()) logFile_ << entry << "\n" << std::flush;
        {
            std::lock_guard<std::mutex> lock(historyMtx_);
            eventHistory_.push_back(std::move(entry));
        }
    }
}

void EventLogger::flushAll() {
    processPendingEvents();
    if (fileOpen_ && logFile_.is_open()) logFile_.flush();
}

std::vector<LogEntry> EventLogger::searchEvents(std::function<bool(const LogEntry&)> predicate) const {
    std::lock_guard<std::mutex> lock(historyMtx_);
    std::vector<LogEntry> results;
    std::copy_if(eventHistory_.begin(), eventHistory_.end(), std::back_inserter(results), predicate);
    return results;
}

std::vector<LogEntry> EventLogger::getRecentEvents(size_t count) const {
    std::lock_guard<std::mutex> lock(historyMtx_);
    std::vector<LogEntry> results;
    if (eventHistory_.empty()) return results;
    size_t start = (eventHistory_.size() > count) ? (eventHistory_.size() - count) : 0;
    for (size_t i = start; i < eventHistory_.size(); ++i) results.push_back(eventHistory_[i]);
    return results;
}

size_t EventLogger::getTotalEventCount() const {
    std::lock_guard<std::mutex> lock(historyMtx_);
    return eventHistory_.size();
}

static EventLogger* g_logger = nullptr;

void setGlobalLogger(EventLogger* logger) {
    g_logger = logger;
}

EventLogger* getGlobalLogger() {
    return g_logger;
}

void LOG_DEBUG(const std::string& source, const std::string& message) {
    if (g_logger) g_logger->log(LogLevel::DEBUG, source, message);
}

void LOG_INFO(const std::string& source, const std::string& message) {
    if (g_logger) g_logger->log(LogLevel::INFO, source, message);
}

void LOG_WARNING(const std::string& source, const std::string& message) {
    if (g_logger) g_logger->log(LogLevel::WARNING, source, message);
}

void LOG_CRITICAL(const std::string& source, const std::string& message) {
    if (g_logger) g_logger->log(LogLevel::CRITICAL, source, message);
}
