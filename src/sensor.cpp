/**
 * @file sensor.cpp
 * @brief Implementation of the Sensor base class and all 6 derived sensors.
 *
 * Each sensor's update() generates a new random value using its per-instance
 * RNG (std::mt19937). In manual mode, update() is a no-op and values are
 * set exclusively through setManualValue().
 *
 * Thread Safety:
 *   Every public method that reads or writes currentValue_ acquires the
 *   per-sensor std::mutex via std::lock_guard. This allows concurrent
 *   access to different sensors without contention.
 *
 * C++ Concepts Demonstrated:
 *   - RAII (lock_guard for mutex)
 *   - Polymorphism (virtual function overrides)
 *   - Operator Overloading (operator<<)
 *   - Static Members (totalSensorCount_)
 *   - Random Number Generation (mt19937 + distributions)
 *   - String Streams (ostringstream for formatting)
 */

#include "sensor.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

// =============================================================================
// ANSI Color Constants (green / yellow / red / bold / reset ONLY)
// =============================================================================

namespace ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
} // namespace ansi

// =============================================================================
// Static Member Initialization
// =============================================================================

std::atomic<int> Sensor::totalSensorCount_{0};

// =============================================================================
// Sensor Base Class — Implementation
// =============================================================================

Sensor::Sensor(std::string name, SensorType type, std::string unit,
               double min, double max)
    : name_(std::move(name))
    , type_(type)
    , unit_(std::move(unit))
    , currentValue_(0.0)
    , minRange_(min)
    , maxRange_(max)
    , manualMode_(false)
    , rng_(std::random_device{}())   // Seed from hardware entropy
{
    ++totalSensorCount_;
}

Sensor::~Sensor() {
    --totalSensorCount_;
}

// --- Thread-Safe Accessors ---

std::string Sensor::getName() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return name_;
}

SensorType Sensor::getType() const {
    // SensorType is immutable after construction — no lock needed
    return type_;
}

std::string Sensor::getUnit() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return unit_;
}

double Sensor::getValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_;
}

// --- Manual Input Mode ---

void Sensor::setManualMode(bool enabled) {
    std::lock_guard<std::mutex> lock(mtx_);
    manualMode_ = enabled;
}

bool Sensor::setManualValue(double value) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (!manualMode_) {
        return false;  // Reject — not in manual mode
    }
    currentValue_ = value;
    return true;
}

bool Sensor::isManualMode() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return manualMode_;
}

// --- Static Members ---

int Sensor::getTotalSensorCount() {
    return totalSensorCount_.load();
}

// --- Operator Overloading ---

std::ostream& operator<<(std::ostream& os, const Sensor& s) {
    std::lock_guard<std::mutex> lock(s.mtx_);

    // Determine status color
    // Note: we call the virtual isCritical() while holding the lock,
    // but since isCritical() also tries to lock, we need to check the
    // value directly here to avoid deadlock.
    bool critical = false;

    // Inline critical check to avoid re-locking
    // (derived classes define their own thresholds)
    switch (s.type_) {
        case SensorType::EngineTemp:
            critical = (s.currentValue_ > 110.0);
            break;
        case SensorType::BatteryVoltage:
            critical = (s.currentValue_ < 10.0);
            break;
        case SensorType::VehicleSpeed:
            critical = (s.currentValue_ > 120.0);
            break;
        case SensorType::TirePressure:
            critical = (s.currentValue_ < 25.0);
            break;
        case SensorType::DoorStatus:
            critical = (s.currentValue_ >= 1.0);  // OPEN
            break;
        case SensorType::Seatbelt:
            critical = (s.currentValue_ >= 1.0);  // UNLOCKED
            break;
    }

    const std::string& color = critical ? ansi::RED : ansi::GREEN;
    const std::string  status = critical ? "[WARNING]" : "[NORMAL] ";

    os << "| " << std::left << std::setw(22) << s.name_
       << "| ";

    // Format value depending on sensor type
    if (s.type_ == SensorType::DoorStatus) {
        os << std::setw(14)
           << (s.currentValue_ >= 1.0 ? "OPEN" : "CLOSED");
    } else if (s.type_ == SensorType::Seatbelt) {
        os << std::setw(14)
           << (s.currentValue_ >= 1.0 ? "UNLOCKED" : "LOCKED");
    } else {
        std::ostringstream valStream;
        valStream << std::fixed << std::setprecision(2) << s.currentValue_
                  << " " << s.unit_;
        os << std::setw(14) << valStream.str();
    }

    os << "| " << color << status << ansi::RESET
       << std::string(14, ' ') << "|";

    return os;
}

// =============================================================================
// EngineTemperatureSensor — 70.0 to 120.0 C, Critical > 110 C
// =============================================================================

EngineTemperatureSensor::EngineTemperatureSensor()
    : Sensor("Engine Temperature", SensorType::EngineTemp, "C", 70.0, 120.0)
    , dist_(70.0, 120.0)
{
    // Generate initial value
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = dist_(rng_);
}

void EngineTemperatureSensor::update() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (manualMode_) return;  // No-op in manual mode
    currentValue_ = dist_(rng_);
}

void EngineTemperatureSensor::display() const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = (currentValue_ > 110.0);
    const std::string& color = critical ? ansi::RED : ansi::GREEN;
    const std::string  status = critical ? "[WARNING]" : "[NORMAL] ";

    std::cout << "| " << std::left << std::setw(22) << name_
              << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left
              << std::setw(4) << unit_
              << "| " << color << status << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string EngineTemperatureSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool EngineTemperatureSensor::isCritical() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ > 110.0;
}

// =============================================================================
// BatterySensor — 9.5 to 14.8 V, Critical < 10.0 V
// =============================================================================

BatterySensor::BatterySensor()
    : Sensor("Battery Voltage", SensorType::BatteryVoltage, "V", 9.5, 14.8)
    , dist_(9.5, 14.8)
{
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = dist_(rng_);
}

void BatterySensor::update() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (manualMode_) return;
    currentValue_ = dist_(rng_);
}

void BatterySensor::display() const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = (currentValue_ < 10.0);
    const std::string& color = critical ? ansi::RED : ansi::GREEN;
    const std::string  status = critical ? "[WARNING]" : "[NORMAL] ";

    std::cout << "| " << std::left << std::setw(22) << name_
              << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left
              << std::setw(4) << unit_
              << "| " << color << status << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string BatterySensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool BatterySensor::isCritical() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ < 10.0;
}

// =============================================================================
// SpeedSensor — 0.0 to 220.0 km/h, Critical > 120 km/h
// =============================================================================

SpeedSensor::SpeedSensor()
    : Sensor("Vehicle Speed", SensorType::VehicleSpeed, "km/h", 0.0, 220.0)
    , dist_(0.0, 220.0)
{
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = dist_(rng_);
}

void SpeedSensor::update() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (manualMode_) return;
    currentValue_ = dist_(rng_);
}

void SpeedSensor::display() const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = (currentValue_ > 120.0);
    const std::string& color = critical ? ansi::YELLOW : ansi::GREEN;
    const std::string  status = critical ? "[WARNING]" : "[NORMAL] ";

    std::cout << "| " << std::left << std::setw(22) << name_
              << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left
              << std::setw(4) << unit_
              << "| " << color << status << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string SpeedSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool SpeedSensor::isCritical() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ > 120.0;
}

// =============================================================================
// TirePressureSensor — 20.0 to 40.0 PSI, Critical < 25 PSI
// =============================================================================

TirePressureSensor::TirePressureSensor()
    : Sensor("Tire Pressure", SensorType::TirePressure, "PSI", 20.0, 40.0)
    , dist_(20.0, 40.0)
{
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = dist_(rng_);
}

void TirePressureSensor::update() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (manualMode_) return;
    currentValue_ = dist_(rng_);
}

void TirePressureSensor::display() const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = (currentValue_ < 25.0);
    const std::string& color = critical ? ansi::YELLOW : ansi::GREEN;
    const std::string  status = critical ? "[WARNING]" : "[NORMAL] ";

    std::cout << "| " << std::left << std::setw(22) << name_
              << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left
              << std::setw(4) << unit_
              << "| " << color << status << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string TirePressureSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool TirePressureSensor::isCritical() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ < 25.0;
}

// =============================================================================
// DoorSensor — CLOSED(0) / OPEN(1), Bernoulli 15% OPEN
// =============================================================================

DoorSensor::DoorSensor()
    : Sensor("Door Status", SensorType::DoorStatus, "", 0.0, 1.0)
    , dist_(0.15)   // 15% probability of OPEN
{
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = dist_(rng_) ? 1.0 : 0.0;
}

void DoorSensor::update() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (manualMode_) return;
    currentValue_ = dist_(rng_) ? 1.0 : 0.0;
}

void DoorSensor::display() const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool isOpen = (currentValue_ >= 1.0);
    const std::string& color = isOpen ? ansi::YELLOW : ansi::GREEN;
    const std::string  status = isOpen ? "[WARNING]" : "[NORMAL] ";
    const std::string  value  = isOpen ? "OPEN" : "CLOSED";

    std::cout << "| " << std::left << std::setw(22) << name_
              << "| " << std::setw(14) << value
              << "| " << color << status << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string DoorSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return (currentValue_ >= 1.0) ? "OPEN" : "CLOSED";
}

bool DoorSensor::isCritical() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ >= 1.0;  // OPEN is flagged; context handled by AlertManager
}

// =============================================================================
// SeatbeltSensor — LOCKED(0) / UNLOCKED(1), Bernoulli 10% UNLOCKED
// =============================================================================

SeatbeltSensor::SeatbeltSensor()
    : Sensor("Seatbelt Status", SensorType::Seatbelt, "", 0.0, 1.0)
    , dist_(0.10)   // 10% probability of UNLOCKED
{
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = dist_(rng_) ? 1.0 : 0.0;
}

void SeatbeltSensor::update() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (manualMode_) return;
    currentValue_ = dist_(rng_) ? 1.0 : 0.0;
}

void SeatbeltSensor::display() const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool isUnlocked = (currentValue_ >= 1.0);
    const std::string& color = isUnlocked ? ansi::YELLOW : ansi::GREEN;
    const std::string  status = isUnlocked ? "[WARNING]" : "[NORMAL] ";
    const std::string  value  = isUnlocked ? "UNLOCKED" : "LOCKED";

    std::cout << "| " << std::left << std::setw(22) << name_
              << "| " << std::setw(14) << value
              << "| " << color << status << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string SeatbeltSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return (currentValue_ >= 1.0) ? "UNLOCKED" : "LOCKED";
}

bool SeatbeltSensor::isCritical() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ >= 1.0;  // UNLOCKED is flagged; context handled by AlertManager
}
