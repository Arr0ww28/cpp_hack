/**
 * @file sensor.hpp
 * @brief Sensor Simulation Framework — Base Architecture
 *
 * Defines the polymorphic sensor hierarchy for the Smart Cabin &
 * Vehicle Health Monitoring System. Provides an abstract Sensor base
 * class with pure virtual interface and 6 concrete sensor implementations.
 *
 * Key Design Decisions:
 *   - Fine-grained locking: each sensor owns its own std::mutex so that
 *     concurrent reads/writes to different sensors never contend.
 *   - Per-instance RNG: each sensor seeds its own std::mt19937 from
 *     std::random_device, ensuring independent and reproducible streams.
 *   - Manual input mode: toggled via setManualMode(). When active, the
 *     sensor's update() becomes a no-op — values are set exclusively
 *     through setManualValue().
 *
 * @note Thread-safe. All public accessors acquire the per-sensor mutex.
 *
 * C++ Concepts Demonstrated:
 *   - Inheritance & Polymorphism (Sensor base -> 6 derived)
 *   - Pure Virtual Functions (update, display, getFormattedValue, isCritical)
 *   - Enum Class (SensorType)
 *   - Operator Overloading (operator<<)
 *   - Static Members (totalSensorCount_)
 *   - RAII (mutex via lock_guard)
 *   - Smart Pointers (intended usage with std::unique_ptr in containers)
 */

#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string>
#include <mutex>
#include <atomic>
#include <random>
#include <iostream>
#include <sstream>
#include <iomanip>

// =============================================================================
// SensorType Enumeration
// =============================================================================

/**
 * @enum SensorType
 * @brief Discriminator for the 6 sensor variants in the system.
 */
enum class SensorType {
    EngineTemp,       ///< Engine coolant temperature (continuous, degrees C)
    BatteryVoltage,   ///< Battery terminal voltage (continuous, Volts)
    VehicleSpeed,     ///< Vehicle ground speed (continuous, km/h)
    DoorStatus,       ///< Door open/closed (discrete, 0=CLOSED 1=OPEN)
    Seatbelt,         ///< Seatbelt locked/unlocked (discrete, 0=LOCKED 1=UNLOCKED)
    TirePressure      ///< Tire pressure (continuous, PSI)
};

// =============================================================================
// Sensor — Abstract Base Class
// =============================================================================

/**
 * @class Sensor
 * @brief Abstract base class defining the common sensor interface.
 *
 * All sensor types inherit from this class and implement the pure virtual
 * methods. The base class provides thread-safe accessors, per-instance RNG
 * infrastructure, and a manual-input override mechanism.
 */
class Sensor {
protected:
    std::string  name_;           ///< Human-readable sensor name
    SensorType   type_;           ///< Enum discriminator
    std::string  unit_;           ///< Measurement unit (e.g., "C", "V", "km/h")
    double       currentValue_;   ///< Current sensor reading
    double       minRange_;       ///< Minimum possible value (for RNG)
    double       maxRange_;       ///< Maximum possible value (for RNG)
    bool         manualMode_;     ///< If true, update() is a no-op

    mutable std::mutex mtx_;      ///< Per-sensor lock for thread safety
    std::mt19937 rng_;            ///< Per-instance Mersenne Twister RNG

    /// Static counter tracking total number of Sensor objects created
    static std::atomic<int> totalSensorCount_;

public:
    /**
     * @brief Construct a new Sensor object.
     * @param name   Human-readable name (e.g., "Engine Temperature")
     * @param type   SensorType enum value
     * @param unit   Unit string (e.g., "C", "V")
     * @param min    Minimum value for random generation
     * @param max    Maximum value for random generation
     */
    Sensor(std::string name, SensorType type, std::string unit,
           double min, double max);

    /**
     * @brief Virtual destructor for safe polymorphic deletion.
     *        Decrements the static sensor count.
     */
    virtual ~Sensor();

    // --- Pure Virtual Interface ---

    /**
     * @brief Generate the next simulated sensor reading.
     *        In manual mode, this is a no-op.
     * @note Thread-safe — implementations must acquire mtx_.
     */
    virtual void update() = 0;

    /**
     * @brief Print a single-line display of this sensor to stdout.
     * @note Thread-safe — implementations must acquire mtx_.
     */
    virtual void display() const = 0;

    /**
     * @brief Get a display-ready string of the current value.
     * @return Formatted string (e.g., "92.30 C", "OPEN", "LOCKED")
     * @note Thread-safe — implementations must acquire mtx_.
     */
    virtual std::string getFormattedValue() const = 0;

    /**
     * @brief Check whether the current reading is in a critical range.
     * @return true if value exceeds the sensor-specific threshold.
     * @note Thread-safe — implementations must acquire mtx_.
     */
    virtual bool isCritical() const = 0;

    // --- Common Accessors (Thread-Safe) ---

    /** @brief Get the human-readable sensor name. */
    std::string getName() const;

    /** @brief Get the SensorType enum value. */
    SensorType getType() const;

    /** @brief Get the measurement unit string. */
    std::string getUnit() const;

    /** @brief Get the current numeric reading (thread-safe copy). */
    double getValue() const;

    // --- Manual Input Mode ---

    /**
     * @brief Enable or disable manual input mode.
     * @param enabled  true = manual (update() no-op), false = simulation (RNG)
     */
    void setManualMode(bool enabled);

    /**
     * @brief Manually set the sensor value (for debugging/testing).
     *        Only effective when manual mode is enabled.
     * @param value  The value to set.
     * @return true if the value was accepted, false if manual mode is off.
     */
    bool setManualValue(double value);

    /** @brief Query whether manual input mode is active. */
    bool isManualMode() const;

    // --- Static Members ---

    /** @brief Get the total count of Sensor objects currently alive. */
    static int getTotalSensorCount();

    // --- Operator Overloading ---

    /**
     * @brief Stream insertion operator for formatted sensor output.
     *        Delegates to getFormattedValue() for the value portion.
     */
    friend std::ostream& operator<<(std::ostream& os, const Sensor& s);
};

// =============================================================================
// Derived Sensor Classes
// =============================================================================

/**
 * @class EngineTemperatureSensor
 * @brief Simulates engine coolant temperature readings.
 *        Range: 70.0 – 120.0 C. Critical threshold: > 110 C.
 */
class EngineTemperatureSensor : public Sensor {
private:
    std::uniform_real_distribution<double> dist_;

public:
    EngineTemperatureSensor();
    void        update() override;
    void        display() const override;
    std::string getFormattedValue() const override;
    bool        isCritical() const override;
};

/**
 * @class BatterySensor
 * @brief Simulates battery terminal voltage readings.
 *        Range: 9.5 – 14.8 V. Critical threshold: < 10.0 V.
 */
class BatterySensor : public Sensor {
private:
    std::uniform_real_distribution<double> dist_;

public:
    BatterySensor();
    void        update() override;
    void        display() const override;
    std::string getFormattedValue() const override;
    bool        isCritical() const override;
};

/**
 * @class SpeedSensor
 * @brief Simulates vehicle ground speed readings.
 *        Range: 0.0 – 220.0 km/h. Critical threshold: > 120 km/h.
 */
class SpeedSensor : public Sensor {
private:
    std::uniform_real_distribution<double> dist_;

public:
    SpeedSensor();
    void        update() override;
    void        display() const override;
    std::string getFormattedValue() const override;
    bool        isCritical() const override;
};

/**
 * @class TirePressureSensor
 * @brief Simulates tire pressure readings.
 *        Range: 20.0 – 40.0 PSI. Critical threshold: < 25 PSI.
 */
class TirePressureSensor : public Sensor {
private:
    std::uniform_real_distribution<double> dist_;

public:
    TirePressureSensor();
    void        update() override;
    void        display() const override;
    std::string getFormattedValue() const override;
    bool        isCritical() const override;
};

/**
 * @class DoorSensor
 * @brief Simulates door open/closed state.
 *        Values: 0 = CLOSED, 1 = OPEN.
 *        Bernoulli distribution with 15% probability of OPEN.
 *        Critical: OPEN (value == 1).
 *        (Context-dependent alert — DOOR_OPEN only if speed > 10 km/h —
 *         is handled by AlertManager, not here.)
 */
class DoorSensor : public Sensor {
private:
    std::bernoulli_distribution dist_;

public:
    DoorSensor();
    void        update() override;
    void        display() const override;
    std::string getFormattedValue() const override;
    bool        isCritical() const override;
};

/**
 * @class SeatbeltSensor
 * @brief Simulates seatbelt locked/unlocked state.
 *        Values: 0 = LOCKED, 1 = UNLOCKED.
 *        Bernoulli distribution with 10% probability of UNLOCKED.
 *        Critical: UNLOCKED (value == 1).
 *        (Context-dependent alert — SEATBELT_WARNING only if speed > 0 —
 *         is handled by AlertManager, not here.)
 */
class SeatbeltSensor : public Sensor {
private:
    std::bernoulli_distribution dist_;

public:
    SeatbeltSensor();
    void        update() override;
    void        display() const override;
    std::string getFormattedValue() const override;
    bool        isCritical() const override;
};

#endif // SENSOR_HPP
