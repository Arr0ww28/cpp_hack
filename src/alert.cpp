#include "alert.hpp"
#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

namespace alert_ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
}

// Generate HH:MM:SS timestamp
static std::string getAlertTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);
    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%H:%M:%S");
    return oss.str();
}

std::atomic<int> Alert::totalAlertCount_{0};

Alert::Alert(AlertType type, AlertSeverity severity, std::string message)
    : type_(type), severity_(severity), message_(std::move(message))
    , timestamp_(getAlertTimestamp()), active_(true)
{
    ++totalAlertCount_;
}

// Copy constructor
Alert::Alert(const Alert& other)
    : type_(other.type_), severity_(other.severity_), message_(other.message_)
    , timestamp_(other.timestamp_), active_(other.active_)
{
}

// Copy assignment
Alert& Alert::operator=(const Alert& other) {
    if (this != &other) {
        type_ = other.type_; severity_ = other.severity_; message_ = other.message_;
        timestamp_ = other.timestamp_; active_ = other.active_;
    }
    return *this;
}

// Move constructor
Alert::Alert(Alert&& other) noexcept
    : type_(other.type_), severity_(other.severity_), message_(std::move(other.message_))
    , timestamp_(std::move(other.timestamp_)), active_(other.active_)
{
}

// Move assignment
Alert& Alert::operator=(Alert&& other) noexcept {
    if (this != &other) {
        type_ = other.type_; severity_ = other.severity_; message_ = std::move(other.message_);
        timestamp_ = std::move(other.timestamp_); active_ = other.active_;
    }
    return *this;
}

AlertType     Alert::getType() const      { return type_; }
AlertSeverity Alert::getSeverity() const   { return severity_; }
std::string   Alert::getMessage() const    { return message_; }
std::string   Alert::getTimestamp() const   { return timestamp_; }
bool          Alert::isActive() const      { return active_; }

void Alert::deactivate() {
    active_ = false;
}

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
        case AlertType::ENGINE_OVERHEAT:   return "ENGINE OVERHEAT";
        case AlertType::LOW_BATTERY:       return "LOW BATTERY";
        case AlertType::HIGH_BATTERY:      return "HIGH BATTERY";
        case AlertType::LOW_TIRE_PRESSURE: return "LOW TIRE PRESSURE";
        case AlertType::HIGH_TIRE_PRESSURE:return "HIGH TIRE PRESSURE";
        case AlertType::OVERSPEED:         return "OVERSPEED";
        case AlertType::DOOR_OPEN:         return "DOOR OPEN";
        case AlertType::SEATBELT_WARNING:  return "SEATBELT WARNING";
    }
    return "UNKNOWN ALERT";
}

std::ostream& operator<<(std::ostream& os, const Alert& alert) {
    std::string color = (alert.severity_ == AlertSeverity::CRITICAL) ? alert_ansi::RED :
                        (alert.severity_ == AlertSeverity::WARNING) ? alert_ansi::YELLOW : alert_ansi::GREEN;

    os << color << "[" << Alert::severityToString(alert.severity_) << "] "
       << Alert::typeToString(alert.type_) << " - " << alert.message_ << alert_ansi::RESET;
    return os;
}

bool Alert::operator==(const Alert& other) const {
    return type_ == other.type_;
}

AlertManager::AlertManager(size_t maxHistory) : maxHistory_(maxHistory) {}

bool AlertManager::isAlertActive(AlertType type) const {
    return std::any_of(activeAlerts_.begin(), activeAlerts_.end(),
        [type](const std::shared_ptr<Alert>& a) { return a->getType() == type && a->isActive(); });
}

void AlertManager::evaluateConditions(const std::vector<std::unique_ptr<Sensor>>& sensors)
{
    double engineTemp = 0.0, batteryVolts = 0.0, speed = 0.0;
    double tirePressure = 0.0, doorStatus = 0.0, seatbeltStatus = 0.0;

    for (const auto& sensor : sensors) {
        switch (sensor->getType()) {
            case SensorType::EngineTemp: engineTemp = sensor->getValue(); break;
            case SensorType::BatteryVoltage: batteryVolts = sensor->getValue(); break;
            case SensorType::VehicleSpeed: speed = sensor->getValue(); break;
            case SensorType::TirePressure: tirePressure = sensor->getValue(); break;
            case SensorType::DoorStatus: doorStatus = sensor->getValue(); break;
            case SensorType::Seatbelt: seatbeltStatus = sensor->getValue(); break;
        }
    }

    std::lock_guard<std::mutex> lock(mtx_);

    // Extract thresholds from current profile
    double engineThreshold = currentProfile_.engineThreshold;
    double batteryThreshold = currentProfile_.batteryThreshold;
    double tireThreshold = currentProfile_.tireThreshold;
    double speedLimit = currentProfile_.speedLimit;
    double doorSpeedThreshold = currentProfile_.doorSpeedThreshold;

    auto checkCondition = [&](bool condition, AlertType type, AlertSeverity severity, const std::string& msg, const std::string& logName) {
        if (condition) {
            if (!isAlertActive(type)) {
                auto alert = std::make_shared<Alert>(type, severity, msg);
                activeAlerts_.push_back(alert);
                alertHistory_.push_back(alert);
                if (severity == AlertSeverity::CRITICAL) LOG_CRITICAL("AlertManager", logName + " triggered: " + msg);
                else LOG_WARNING("AlertManager", logName + " triggered: " + msg);
            }
        } else {
            if (isAlertActive(type)) {
                LOG_INFO("AlertManager", logName + " cleared");
            }
            activeAlerts_.erase(std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
                [type](const std::shared_ptr<Alert>& a) { return a->getType() == type; }), activeAlerts_.end());
        }
    };

    // Engine Overheat
    std::ostringstream engineMsg;
    engineMsg << std::fixed << std::setprecision(1) << "Temperature " << engineTemp << " C exceeds " << engineThreshold << " C";
    checkCondition(engineTemp > engineThreshold, AlertType::ENGINE_OVERHEAT, AlertSeverity::CRITICAL, engineMsg.str(), "ENGINE OVERHEAT");

    // Low/High Battery
    std::ostringstream battMsg;
    if (batteryVolts < batteryThreshold) {
        battMsg << std::fixed << std::setprecision(1) << "Voltage " << batteryVolts << " V below " << batteryThreshold << " V";
        checkCondition(true, AlertType::LOW_BATTERY, AlertSeverity::WARNING, battMsg.str(), "LOW BATTERY");
        checkCondition(false, AlertType::HIGH_BATTERY, AlertSeverity::WARNING, "", "");
    } else if (batteryVolts > 15.0) {
        battMsg << std::fixed << std::setprecision(1) << "Voltage " << batteryVolts << " V above 15.0 V";
        checkCondition(true, AlertType::HIGH_BATTERY, AlertSeverity::WARNING, battMsg.str(), "HIGH BATTERY");
        checkCondition(false, AlertType::LOW_BATTERY, AlertSeverity::WARNING, "", "");
    } else {
        checkCondition(false, AlertType::LOW_BATTERY, AlertSeverity::WARNING, "", "");
        checkCondition(false, AlertType::HIGH_BATTERY, AlertSeverity::WARNING, "", "");
    }

    // Low/High Tire Pressure
    std::ostringstream tireMsg;
    if (tirePressure < tireThreshold) {
        tireMsg << std::fixed << std::setprecision(1) << "Pressure " << tirePressure << " PSI below " << tireThreshold << " PSI";
        checkCondition(true, AlertType::LOW_TIRE_PRESSURE, AlertSeverity::WARNING, tireMsg.str(), "LOW TIRE PRESSURE");
        checkCondition(false, AlertType::HIGH_TIRE_PRESSURE, AlertSeverity::WARNING, "", "");
    } else if (tirePressure > 40.0) {
        tireMsg << std::fixed << std::setprecision(1) << "Pressure " << tirePressure << " PSI above 40.0 PSI";
        checkCondition(true, AlertType::HIGH_TIRE_PRESSURE, AlertSeverity::WARNING, tireMsg.str(), "HIGH TIRE PRESSURE");
        checkCondition(false, AlertType::LOW_TIRE_PRESSURE, AlertSeverity::WARNING, "", "");
    } else {
        checkCondition(false, AlertType::LOW_TIRE_PRESSURE, AlertSeverity::WARNING, "", "");
        checkCondition(false, AlertType::HIGH_TIRE_PRESSURE, AlertSeverity::WARNING, "", "");
    }

    // Overspeed
    std::ostringstream speedMsg;
    speedMsg << std::fixed << std::setprecision(1) << "Speed " << speed << " km/h exceeds limit of " << speedLimit << " km/h";
    checkCondition(speed > speedLimit, AlertType::OVERSPEED, AlertSeverity::WARNING, speedMsg.str(), "OVERSPEED");

    // Door Open While Moving
    std::ostringstream doorMsg;
    doorMsg << std::fixed << std::setprecision(1) << "Door OPEN at " << speed << " km/h (Limit: " << doorSpeedThreshold << ")";
    checkCondition(doorStatus == 1.0 && speed > doorSpeedThreshold, AlertType::DOOR_OPEN, AlertSeverity::CRITICAL, doorMsg.str(), "DOOR OPEN");

    // Seatbelt Unbuckled While Moving
    std::ostringstream beltMsg;
    beltMsg << std::fixed << std::setprecision(1) << "Seatbelt UNLOCKED at " << speed << " km/h";
    checkCondition(seatbeltStatus == 1.0 && speed > 0.0, AlertType::SEATBELT_WARNING, AlertSeverity::WARNING, beltMsg.str(), "SEATBELT WARNING");

    // Trim history
    while (alertHistory_.size() > maxHistory_) {
        alertHistory_.pop_front();
    }
}

void AlertManager::addAlert(std::shared_ptr<Alert> alert) {
    std::lock_guard<std::mutex> lock(mtx_);
    activeAlerts_.push_back(alert);
    alertHistory_.push_back(alert);
    while (alertHistory_.size() > maxHistory_) alertHistory_.pop_front();
}

void AlertManager::clearAlert(AlertType type) {
    std::lock_guard<std::mutex> lock(mtx_);
    activeAlerts_.erase(std::remove_if(activeAlerts_.begin(), activeAlerts_.end(),
        [type](const std::shared_ptr<Alert>& a) { return a->getType() == type; }), activeAlerts_.end());
}

void AlertManager::clearAllAlerts() {
    std::lock_guard<std::mutex> lock(mtx_);
    activeAlerts_.clear();
    LOG_INFO("AlertManager", "All active alerts cleared");
}

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

std::vector<std::shared_ptr<Alert>> AlertManager::filterAlerts(std::function<bool(const Alert&)> predicate) const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::vector<std::shared_ptr<Alert>> results;
    std::copy_if(activeAlerts_.begin(), activeAlerts_.end(), std::back_inserter(results),
        [&predicate](const std::shared_ptr<Alert>& a) { return predicate(*a); });
    return results;
}
