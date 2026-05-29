#include "health.hpp"
#include "logger.hpp"

HealthMonitor::HealthMonitor(std::chrono::milliseconds timeout)
    : timeout_(timeout) {}

void HealthMonitor::ping(const std::string& componentName) {
    std::lock_guard<std::mutex> lock(mtx_);
    heartbeats_[componentName] = std::chrono::steady_clock::now();
}

void HealthMonitor::checkHealth(AlertManager& alertMgr) {
    std::lock_guard<std::mutex> lock(mtx_);
    auto now = std::chrono::steady_clock::now();

    for (const auto& [component, lastPing] : heartbeats_) {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPing);
        if (elapsed > timeout_) {
            std::string msg = component + " is unresponsive (no heartbeat for " + std::to_string(elapsed.count()) + "ms)";
            
            // Only add alert if it's not already active to avoid spamming
            if (!alertMgr.isAlertActive(AlertType::ECU_FAULT)) {
                LOG_CRITICAL("HealthMonitor", "WATCHDOG TIMEOUT: " + msg);
                alertMgr.addAlert(std::make_shared<Alert>(AlertType::ECU_FAULT, AlertSeverity::CRITICAL, msg));
            }
        }
    }
}
