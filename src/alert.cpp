/**
 * @file alert.cpp
 * @brief Implementation of the Alert class and AlertManager.
 *
 * Alert implements the Rule of Five for proper value semantics and
 * operator<< for ANSI-colored formatted output. AlertManager evaluates
 * sensor conditions with cross-referencing (e.g., door + speed) and
 * manages the alert lifecycle with thread-safe containers.
 *
 * All functions include LOG_ calls for full debug tracing.
 *
 * C++ Concepts Demonstrated:
 *   - Copy & Move Semantics (Alert Rule of Five)
 *   - Operator Overloading (operator<< and operator==)
 *   - Static Members (totalAlertCount_)
 *   - Smart Pointers (shared_ptr for shared alert ownership)
 *   - STL Containers (vector, deque)
 *   - Lambdas (filterAlerts, std::find_if, std::remove_if)
 *   - Mutex / lock_guard (thread-safe access)
 */

#include "alert.hpp"
#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

// =============================================================================
// ANSI Color Constants
// =============================================================================

namespace alert_ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
} // namespace alert_ansi

// =============================================================================
// Timestamp Helper
// =============================================================================

static std::string getAlertTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    return oss.str();
}

// =============================================================================
// Static Member Initialization
// =============================================================================

std::atomic<int> Alert::totalAlertCount_{0};

// =============================================================================
// Alert — Constructor
// =============================================================================

Alert::Alert(AlertType type, AlertSeverity severity, std::string message)
    : type_(type)
    , severity_(severity)
    , message_(std::move(message))
    , timestamp_(getAlertTimestamp())
    , active_(true)
{
    ++totalAlertCount_;
    LOG_INFO("Alert", "Created: [" + severityToString(severity_) + "] "
             + typeToString(type_) + " - " + message_);
}

// =============================================================================
// Alert — Rule of Five: Copy & Move Semantics
// =============================================================================

Alert::Alert(const Alert& other)
    : type_(other.type_)
    , severity_(other.severity_)
    , message_(other.message_)
    , timestamp_(other.timestamp_)
    , active_(other.active_)
{
    // Note: copy does NOT increment totalAlertCount_ — it's a copy, not a new alert
    LOG_DEBUG("Alert", "Copy constructed: " + typeToString(type_));
}

Alert& Alert::operator=(const Alert& other) {
    if (this != &other) {
        type_      = other.type_;
        severity_  = other.severity_;
        message_   = other.message_;
        timestamp_ = other.timestamp_;
        active_    = other.active_;
        LOG_DEBUG("Alert", "Copy assigned: " + typeToString(type_));
    }
    return *this;
}

Alert::Alert(Alert&& other) noexcept
    : type_(other.type_)
    , severity_(other.severity_)
    , message_(std::move(other.message_))
    , timestamp_(std::move(other.timestamp_))
    , active_(other.active_)
{
    LOG_DEBUG("Alert", "Move constructed: " + typeToString(type_));
}

Alert& Alert::operator=(Alert&& other) noexcept {
    if (this != &other) {
        type_      = other.type_;
        severity_  = other.severity_;
        message_   = std::move(other.message_);
        timestamp_ = std::move(other.timestamp_);
        active_    = other.active_;
        LOG_DEBUG("Alert", "Move assigned: " + typeToString(type_));
    }
    return *this;
}

// =============================================================================
// Alert — Accessors
// =============================================================================

AlertType     Alert::getType() const      { return type_; }
AlertSeverity Alert::getSeverity() const   { return severity_; }
std::string   Alert::getMessage() const    { return message_; }
std::string   Alert::getTimestamp() const   { return timestamp_; }
bool          Alert::isActive() const      { return active_; }

void Alert::deactivate() {
    active_ = false;
    LOG_DEBUG("Alert", "Deactivated: " + typeToString(type_));
}

// =============================================================================
// Alert — Static Members
// =============================================================================

int Alert::getTotalAlertCount() {
    return totalAlertCount_.load();
}

std::string Alert::severityToString(AlertSeverity s) {
    switch (s) {
        case AlertSeverity::INFO:     return "INFO";
        case AlertSeverity::WARNING:  return "WARNING";
        case AlertSeverity::CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN";
}

std::string Alert::typeToString(AlertType t) {
    switch (t) {
        case AlertType::ENGINE_OVERHEAT:     return "CRITICAL ENGINE OVERHEAT";
        case AlertType::LOW_BATTERY:         return "LOW BATTERY WARNING";
        case AlertType::LOW_TIRE_PRESSURE:   return "LOW TIRE PRESSURE";
        case AlertType::OVERSPEED:           return "OVERSPEED WARNING";
        case AlertType::DOOR_OPEN:           return "DOOR OPEN WARNING";
        case AlertType::SEATBELT_WARNING:    return "SEATBELT WARNING";
    }
    return "UNKNOWN ALERT";
}

// =============================================================================
// Alert — Operator Overloading
// =============================================================================

std::ostream& operator<<(std::ostream& os, const Alert& alert) {
    // Color by severity: RED = CRITICAL, YELLOW = WARNING, GREEN = INFO
    std::string color;
    switch (alert.severity_) {
        case AlertSeverity::CRITICAL: color = alert_ansi::RED;    break;
        case AlertSeverity::WARNING:  color = alert_ansi::YELLOW; break;
        case AlertSeverity::INFO:     color = alert_ansi::GREEN;  break;
    }

    os << color << "[" << Alert::severityToString(alert.severity_) << "] "
       << Alert::typeToString(alert.type_) << " - " << alert.message_
       << alert_ansi::RESET;

    return os;
}

bool Alert::operator==(const Alert& other) const {
    return type_ == other.type_;
}

// =============================================================================
// AlertManager — Constructor
// =============================================================================

AlertManager::AlertManager(size_t maxHistory)
    : maxHistory_(maxHistory)
{
    LOG_INFO("AlertManager", "Initialized with max history: " + std::to_string(maxHistory));
}

// =============================================================================
// AlertManager — Internal Helpers
// =============================================================================

bool AlertManager::isAlertActive(AlertType type) const {
    // Must be called with mtx_ held
    return std::any_of(activeAlerts_.begin(), activeAlerts_.end(),
        [type](const std::shared_ptr<Alert>& a) {
            return a->getType() == type && a->isActive();
        });
}

// =============================================================================
// AlertManager — Condition Evaluation
// =============================================================================

void AlertManager::evaluateConditions(
    const std::vector<std::unique_ptr<Sensor>>& sensors,
    double engineThreshold, double batteryThreshold,
    double tireThreshold, double speedLimit,
    double doorSpeedThreshold)
{
    LOG_DEBUG("AlertManager", "evaluateConditions() called");

    // Collect current sensor values (thread-safe reads)
    double engineTemp    = 0.0;
    double batteryVolts  = 0.0;
    double speed         = 0.0;
    double tirePressure  = 0.0;
    double doorStatus    = 0.0;
    double seatbeltStatus = 0.0;

    for (const auto& sensor : sensors) {
        switch (sensor->getType()) {
            case SensorType::EngineTemp:
                engineTemp = sensor->getValue();
                break;
            case SensorType::BatteryVoltage:
                batteryVolts = sensor->getValue();
                break;
            case SensorType::VehicleSpeed:
                speed = sensor->getValue();
                break;
            case SensorType::TirePressure:
                tirePressure = sensor->getValue();
                break;
            case SensorType::DoorStatus:
                doorStatus = sensor->getValue();
                break;
            case SensorType::Seatbelt:
                seatbeltStatus = sensor->getValue();
                break;
        }
    }

    std::lock_guard<std::mutex> lock(mtx_);

    // --- Engine Overheat: temp > threshold ---
    if (engineTemp > engineThreshold) {
        if (!isAlertActive(AlertType::ENGINE_OVERHEAT)) {
            std::ostringstream msg;
            msg << std::fixed << std::setprecision(1)
                << "Temperature " << engineTemp << " C exceeds " << engineThreshold << " C";
            auto alert = std::make_shared<Alert>(
                AlertType::ENGINE_OVERHEAT, AlertSeverity::CRITICAL, msg.str());
            activeAlerts_.push_back(alert);
            alertHistory_.push_back(alert);
            LOG_CRITICAL("AlertManager", "ENGINE OVERHEAT triggered: " + msg.str());
        }
    } else {
        if (isAlertActive(AlertType::ENGINE_OVERHEAT)) {
            LOG_INFO("AlertManager", "ENGINE OVERHEAT cleared — temp normal");
        }
        // Remove from active (lambda with std::remove_if)
        activeAlerts_.erase(
            std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [](const std::shared_ptr<Alert>& a) {
                    return a->getType() == AlertType::ENGINE_OVERHEAT;
                }),
            activeAlerts_.end());
    }

    // --- Low Battery: voltage < threshold ---
    if (batteryVolts < batteryThreshold) {
        if (!isAlertActive(AlertType::LOW_BATTERY)) {
            std::ostringstream msg;
            msg << std::fixed << std::setprecision(1)
                << "Voltage " << batteryVolts << " V below " << batteryThreshold << " V";
            auto alert = std::make_shared<Alert>(
                AlertType::LOW_BATTERY, AlertSeverity::WARNING, msg.str());
            activeAlerts_.push_back(alert);
            alertHistory_.push_back(alert);
            LOG_WARNING("AlertManager", "LOW BATTERY triggered: " + msg.str());
        }
    } else {
        if (isAlertActive(AlertType::LOW_BATTERY)) {
            LOG_INFO("AlertManager", "LOW BATTERY cleared — voltage normal");
        }
        activeAlerts_.erase(
            std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [](const std::shared_ptr<Alert>& a) {
                    return a->getType() == AlertType::LOW_BATTERY;
                }),
            activeAlerts_.end());
    }

    // --- Low Tire Pressure: pressure < threshold ---
    if (tirePressure < tireThreshold) {
        if (!isAlertActive(AlertType::LOW_TIRE_PRESSURE)) {
            std::ostringstream msg;
            msg << std::fixed << std::setprecision(1)
                << "Pressure " << tirePressure << " PSI below " << tireThreshold << " PSI";
            auto alert = std::make_shared<Alert>(
                AlertType::LOW_TIRE_PRESSURE, AlertSeverity::WARNING, msg.str());
            activeAlerts_.push_back(alert);
            alertHistory_.push_back(alert);
            LOG_WARNING("AlertManager", "LOW TIRE PRESSURE triggered: " + msg.str());
        }
    } else {
        if (isAlertActive(AlertType::LOW_TIRE_PRESSURE)) {
            LOG_INFO("AlertManager", "LOW TIRE PRESSURE cleared — pressure normal");
        }
        activeAlerts_.erase(
            std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [](const std::shared_ptr<Alert>& a) {
                    return a->getType() == AlertType::LOW_TIRE_PRESSURE;
                }),
            activeAlerts_.end());
    }

    // --- Overspeed: speed > limit ---
    if (speed > speedLimit) {
        if (!isAlertActive(AlertType::OVERSPEED)) {
            std::ostringstream msg;
            msg << std::fixed << std::setprecision(1)
                << "Speed " << speed << " km/h exceeds " << speedLimit << " km/h";
            auto alert = std::make_shared<Alert>(
                AlertType::OVERSPEED, AlertSeverity::WARNING, msg.str());
            activeAlerts_.push_back(alert);
            alertHistory_.push_back(alert);
            LOG_WARNING("AlertManager", "OVERSPEED triggered: " + msg.str());
        }
    } else {
        if (isAlertActive(AlertType::OVERSPEED)) {
            LOG_INFO("AlertManager", "OVERSPEED cleared — speed normal");
        }
        activeAlerts_.erase(
            std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [](const std::shared_ptr<Alert>& a) {
                    return a->getType() == AlertType::OVERSPEED;
                }),
            activeAlerts_.end());
    }

    // --- Door Open: door open AND speed > threshold ---
    if (doorStatus >= 1.0 && speed > doorSpeedThreshold) {
        if (!isAlertActive(AlertType::DOOR_OPEN)) {
            std::ostringstream msg;
            msg << std::fixed << std::setprecision(1)
                << "Door OPEN at " << speed << " km/h (threshold: "
                << doorSpeedThreshold << " km/h)";
            auto alert = std::make_shared<Alert>(
                AlertType::DOOR_OPEN, AlertSeverity::CRITICAL, msg.str());
            activeAlerts_.push_back(alert);
            alertHistory_.push_back(alert);
            LOG_CRITICAL("AlertManager", "DOOR OPEN triggered: " + msg.str());
        }
    } else {
        if (isAlertActive(AlertType::DOOR_OPEN)) {
            LOG_INFO("AlertManager", "DOOR OPEN cleared — door closed or speed low");
        }
        activeAlerts_.erase(
            std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [](const std::shared_ptr<Alert>& a) {
                    return a->getType() == AlertType::DOOR_OPEN;
                }),
            activeAlerts_.end());
    }

    // --- Seatbelt: unlocked AND speed > 0 ---
    if (seatbeltStatus >= 1.0 && speed > 0.0) {
        if (!isAlertActive(AlertType::SEATBELT_WARNING)) {
            std::ostringstream msg;
            msg << std::fixed << std::setprecision(1)
                << "Seatbelt UNLOCKED while moving at " << speed << " km/h";
            auto alert = std::make_shared<Alert>(
                AlertType::SEATBELT_WARNING, AlertSeverity::WARNING, msg.str());
            activeAlerts_.push_back(alert);
            alertHistory_.push_back(alert);
            LOG_WARNING("AlertManager", "SEATBELT WARNING triggered: " + msg.str());
        }
    } else {
        if (isAlertActive(AlertType::SEATBELT_WARNING)) {
            LOG_INFO("AlertManager", "SEATBELT WARNING cleared — seatbelt locked or stopped");
        }
        activeAlerts_.erase(
            std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [](const std::shared_ptr<Alert>& a) {
                    return a->getType() == AlertType::SEATBELT_WARNING;
                }),
            activeAlerts_.end());
    }

    // Trim history if exceeding max
    while (alertHistory_.size() > maxHistory_) {
        alertHistory_.pop_front();
    }

    LOG_DEBUG("AlertManager", "evaluateConditions() complete — active: "
              + std::to_string(activeAlerts_.size()) + ", history: "
              + std::to_string(alertHistory_.size()));
}

// =============================================================================
// AlertManager — Alert Lifecycle
// =============================================================================

void AlertManager::addAlert(std::shared_ptr<Alert> alert) {
    std::lock_guard<std::mutex> lock(mtx_);
    activeAlerts_.push_back(alert);
    alertHistory_.push_back(alert);
    LOG_DEBUG("AlertManager", "addAlert(): " + Alert::typeToString(alert->getType()));

    while (alertHistory_.size() > maxHistory_) {
        alertHistory_.pop_front();
    }
}

void AlertManager::clearAlert(AlertType type) {
    std::lock_guard<std::mutex> lock(mtx_);
    activeAlerts_.erase(
        std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
            [type](const std::shared_ptr<Alert>& a) {
                return a->getType() == type;
            }),
        activeAlerts_.end());
    LOG_DEBUG("AlertManager", "clearAlert(): " + Alert::typeToString(type));
}

void AlertManager::clearAllAlerts() {
    std::lock_guard<std::mutex> lock(mtx_);
    activeAlerts_.clear();
    LOG_INFO("AlertManager", "All active alerts cleared");
}

// =============================================================================
// AlertManager — Accessors (Thread-Safe Snapshots)
// =============================================================================

std::vector<std::shared_ptr<Alert>> AlertManager::getActiveAlerts() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return activeAlerts_;
}

std::deque<std::shared_ptr<Alert>> AlertManager::getAlertHistory() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return alertHistory_;
}

size_t AlertManager::getActiveAlertCount() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return activeAlerts_.size();
}

// =============================================================================
// AlertManager — Lambda-Based Filtering
// =============================================================================

std::vector<std::shared_ptr<Alert>> AlertManager::filterAlerts(
    std::function<bool(const Alert&)> predicate) const
{
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<std::shared_ptr<Alert>> results;

    // Use STL algorithm with lambda
    std::copy_if(activeAlerts_.begin(), activeAlerts_.end(),
        std::back_inserter(results),
        [&predicate](const std::shared_ptr<Alert>& a) {
            return predicate(*a);
        });

    LOG_DEBUG("AlertManager", "filterAlerts(): " + std::to_string(results.size())
              + " matches from " + std::to_string(activeAlerts_.size()) + " active");
    return results;
}
