#ifndef ALERT_HPP
#define ALERT_HPP

#include "sensor.hpp"

#include <string>
#include <vector>
#include <deque>
#include <memory>
#include <mutex>
#include <atomic>
#include <functional>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Alert severity levels
enum class AlertSeverity {
    INFO, WARNING, CRITICAL
};

// Discriminator for alert conditions
enum class AlertType {
    ENGINE_OVERHEAT, LOW_BATTERY, LOW_TIRE_PRESSURE,
    OVERSPEED, DOOR_OPEN, SEATBELT_WARNING
};

// Represents a single vehicle alert event
class Alert {
private:
    AlertType     type_;
    AlertSeverity severity_;
    std::string   message_;
    std::string   timestamp_;
    bool          active_;

    static std::atomic<int> totalAlertCount_;

public:
    Alert(AlertType type, AlertSeverity severity, std::string message);
    ~Alert() = default;

    Alert(const Alert& other);
    Alert& operator=(const Alert& other);
    Alert(Alert&& other) noexcept;
    Alert& operator=(Alert&& other) noexcept;

    AlertType     getType() const;
    AlertSeverity getSeverity() const;
    std::string   getMessage() const;
    std::string   getTimestamp() const;
    bool          isActive() const;
    void          deactivate();

    static int getTotalAlertCount();
    static std::string severityToString(AlertSeverity s);
    static std::string typeToString(AlertType t);

    friend std::ostream& operator<<(std::ostream& os, const Alert& alert);
    bool operator==(const Alert& other) const;
};

// Evaluates sensor conditions and manages alert lifecycle
class AlertManager {
private:
    std::vector<std::shared_ptr<Alert>> activeAlerts_;
    std::deque<std::shared_ptr<Alert>>  alertHistory_;
    mutable std::mutex mtx_;
    size_t maxHistory_;

    bool isAlertActive(AlertType type) const;

public:
    explicit AlertManager(size_t maxHistory = 100);
    ~AlertManager() = default;

    void evaluateConditions(const std::vector<std::unique_ptr<Sensor>>& sensors,
                            double engineThreshold, double batteryThreshold,
                            double tireThreshold, double speedLimit,
                            double doorSpeedThreshold);

    void addAlert(std::shared_ptr<Alert> alert);
    void clearAlert(AlertType type);
    void clearAllAlerts();

    std::vector<std::shared_ptr<Alert>> getActiveAlerts() const;
    std::deque<std::shared_ptr<Alert>> getAlertHistory() const;
    size_t getActiveAlertCount() const;

    std::vector<std::shared_ptr<Alert>> filterAlerts(
        std::function<bool(const Alert&)> predicate) const;
};

#endif // ALERT_HPP
