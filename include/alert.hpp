/**
 * @file alert.hpp
 * @brief Alert Management Module — Alert Generation & Condition Evaluation
 *
 * Defines the Alert class (with copy/move semantics, operator overloading,
 * static member tracking) and the AlertManager class (condition evaluation,
 * lambda-based filtering, thread-safe alert lifecycle).
 *
 * C++ Concepts Demonstrated:
 *   - Enum Class (AlertSeverity, AlertType)
 *   - Copy & Move Semantics (Rule of Five on Alert)
 *   - Operator Overloading (operator<< and operator== on Alert)
 *   - Static Members (Alert::totalAlertCount_)
 *   - Smart Pointers (shared_ptr<Alert> in containers)
 *   - STL Containers (vector, deque)
 *   - Lambdas (filterAlerts predicate)
 *   - Mutex / lock_guard (thread-safe alert access)
 */

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

// =============================================================================
// Alert Enumerations
// =============================================================================

/**
 * @enum AlertSeverity
 * @brief Severity classification for vehicle alerts.
 */
enum class AlertSeverity {
    INFO,       ///< Informational (no action needed)
    WARNING,    ///< Warning (attention recommended)
    CRITICAL    ///< Critical (immediate action required)
};

/**
 * @enum AlertType
 * @brief Discriminator for the 6 alert conditions.
 */
enum class AlertType {
    ENGINE_OVERHEAT,     ///< Engine temperature > threshold
    LOW_BATTERY,         ///< Battery voltage < threshold
    LOW_TIRE_PRESSURE,   ///< Tire pressure < threshold
    OVERSPEED,           ///< Vehicle speed > threshold
    DOOR_OPEN,           ///< Door open while speed > threshold
    SEATBELT_WARNING     ///< Seatbelt unlocked while moving
};

// =============================================================================
// Alert Class
// =============================================================================

/**
 * @class Alert
 * @brief Represents a single vehicle alert event.
 *
 * Supports the Rule of Five (copy ctor, copy assign, move ctor, move assign,
 * destructor) for proper value semantics. Includes operator<< for formatted
 * ANSI-colored output and operator== for type-based comparison.
 */
class Alert {
private:
    AlertType     type_;        ///< What kind of alert
    AlertSeverity severity_;    ///< How severe
    std::string   message_;     ///< Human-readable description
    std::string   timestamp_;   ///< When the alert was generated
    bool          active_;      ///< Is the alert currently active

    /// Static counter tracking total number of Alert objects ever created
    static std::atomic<int> totalAlertCount_;

public:
    /**
     * @brief Construct a new Alert with auto-generated timestamp.
     * @param type      AlertType enum value
     * @param severity  AlertSeverity enum value
     * @param message   Human-readable alert description
     */
    Alert(AlertType type, AlertSeverity severity, std::string message);

    /// Default destructor
    ~Alert() = default;

    // --- Rule of Five: Copy & Move Semantics ---

    /** @brief Copy constructor (deep copy of all members). */
    Alert(const Alert& other);

    /** @brief Copy assignment operator. */
    Alert& operator=(const Alert& other);

    /** @brief Move constructor (transfers ownership of strings). */
    Alert(Alert&& other) noexcept;

    /** @brief Move assignment operator. */
    Alert& operator=(Alert&& other) noexcept;

    // --- Accessors ---

    AlertType     getType() const;
    AlertSeverity getSeverity() const;
    std::string   getMessage() const;
    std::string   getTimestamp() const;
    bool          isActive() const;
    void          deactivate();

    // --- Static Members ---

    /** @brief Get total number of Alert objects ever created. */
    static int getTotalAlertCount();

    /** @brief Convert AlertSeverity enum to display string. */
    static std::string severityToString(AlertSeverity s);

    /** @brief Convert AlertType enum to display string. */
    static std::string typeToString(AlertType t);

    // --- Operator Overloading ---

    /**
     * @brief Stream insertion for ANSI-colored alert display.
     *        Uses RED for CRITICAL, YELLOW for WARNING, GREEN for INFO.
     */
    friend std::ostream& operator<<(std::ostream& os, const Alert& alert);

    /**
     * @brief Equality comparison based on AlertType (for dedup).
     */
    bool operator==(const Alert& other) const;
};

// =============================================================================
// AlertManager Class
// =============================================================================

/**
 * @class AlertManager
 * @brief Evaluates sensor conditions and manages alert lifecycle.
 *
 * Thread-safe. All public methods acquire the internal mutex.
 * Maintains both active alerts (current) and a bounded history deque.
 */
class AlertManager {
private:
    std::vector<std::shared_ptr<Alert>> activeAlerts_;    ///< Currently active
    std::deque<std::shared_ptr<Alert>>  alertHistory_;    ///< Historical (bounded)
    mutable std::mutex mtx_;                              ///< Thread safety
    size_t maxHistory_;                                   ///< Max history entries

    /**
     * @brief Check if an alert of the given type is already active.
     *        Must be called with mtx_ held.
     */
    bool isAlertActive(AlertType type) const;

public:
    /**
     * @brief Construct AlertManager with configurable history limit.
     * @param maxHistory  Maximum number of historical alerts to retain.
     */
    explicit AlertManager(size_t maxHistory = 100);
    ~AlertManager() = default;

    /**
     * @brief Evaluate all sensor readings against alert thresholds.
     *
     * Cross-references speed for context-dependent alerts:
     *   - DOOR_OPEN only if speed > doorSpeedThreshold
     *   - SEATBELT_WARNING only if speed > 0
     *
     * @param sensors             Vector of sensor unique_ptrs
     * @param engineThreshold     Engine temp critical threshold (C)
     * @param batteryThreshold    Battery voltage low threshold (V)
     * @param tireThreshold       Tire pressure low threshold (PSI)
     * @param speedLimit          Speed warning threshold (km/h)
     * @param doorSpeedThreshold  Min speed for door-open alert (km/h)
     */
    void evaluateConditions(const std::vector<std::unique_ptr<Sensor>>& sensors,
                            double engineThreshold, double batteryThreshold,
                            double tireThreshold, double speedLimit,
                            double doorSpeedThreshold);

    // --- Alert Lifecycle ---

    /** @brief Add a new alert (marks active, appends to history). */
    void addAlert(std::shared_ptr<Alert> alert);

    /** @brief Deactivate and remove an alert of the given type. */
    void clearAlert(AlertType type);

    /** @brief Deactivate and remove all active alerts. */
    void clearAllAlerts();

    // --- Accessors (Thread-Safe) ---

    /** @brief Get a snapshot of all currently active alerts. */
    std::vector<std::shared_ptr<Alert>> getActiveAlerts() const;

    /** @brief Get a snapshot of the alert history. */
    std::deque<std::shared_ptr<Alert>> getAlertHistory() const;

    /** @brief Get count of currently active alerts. */
    size_t getActiveAlertCount() const;

    // --- Lambda-Based Filtering ---

    /**
     * @brief Filter active alerts using a predicate function.
     * @param predicate  Lambda returning true for matching alerts.
     * @return Vector of matching alerts.
     */
    std::vector<std::shared_ptr<Alert>> filterAlerts(
        std::function<bool(const Alert&)> predicate) const;
};

#endif // ALERT_HPP
