#ifndef HEALTH_HPP
#define HEALTH_HPP

#include "alert.hpp"
#include <string>
#include <map>
#include <mutex>
#include <chrono>

class HealthMonitor {
private:
    std::map<std::string, std::chrono::steady_clock::time_point> heartbeats_;
    mutable std::mutex mtx_;
    std::chrono::milliseconds timeout_;

public:
    explicit HealthMonitor(std::chrono::milliseconds timeout = std::chrono::milliseconds(2000));
    ~HealthMonitor() = default;

    // Threads call this to signal they are alive
    void ping(const std::string& componentName);

    // Watchdog thread calls this periodically to check for dead threads
    void checkHealth(AlertManager& alertMgr);
};

#endif // HEALTH_HPP
