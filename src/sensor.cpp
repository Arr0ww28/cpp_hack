#include "sensor.hpp"
#include "logger.hpp"

namespace ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
}

std::atomic<int> Sensor::totalSensorCount_{0};

Sensor::Sensor(std::string name, SensorType type, std::string unit, double defaultVal)
    : name_(std::move(name)), type_(type), unit_(std::move(unit)), currentValue_(defaultVal)
{
    ++totalSensorCount_;
}

Sensor::~Sensor() {
    --totalSensorCount_;
}

std::string Sensor::getName() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return name_;
}

SensorType Sensor::getType() const {
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

void Sensor::setValue(double value) {
    std::lock_guard<std::mutex> lock(mtx_);
    currentValue_ = value;
}

int Sensor::getTotalSensorCount() {
    return totalSensorCount_.load();
}

std::ostream& operator<<(std::ostream& os, const Sensor& s) {
    std::lock_guard<std::mutex> lock(s.mtx_);

    bool critical = false;
    switch (s.type_) {
        case SensorType::EngineTemp:     critical = (s.currentValue_ > 110.0); break;
        case SensorType::BatteryVoltage: critical = (s.currentValue_ < 10.0); break;
        case SensorType::VehicleSpeed:   critical = (s.currentValue_ > 120.0); break;
        case SensorType::TirePressure:   critical = (s.currentValue_ < 25.0); break;
        case SensorType::DoorStatus:     critical = (s.currentValue_ >= 1.0); break;
        case SensorType::Seatbelt:       critical = (s.currentValue_ >= 1.0); break;
    }

    const std::string& color = critical ? ansi::RED : ansi::GREEN;
    const std::string  status = critical ? "[WARNING]" : "[NORMAL] ";

    os << "| " << std::left << std::setw(22) << s.name_ << "| ";

    if (s.type_ == SensorType::DoorStatus) {
        os << std::setw(14) << (s.currentValue_ >= 1.0 ? "OPEN" : "CLOSED");
    } else if (s.type_ == SensorType::Seatbelt) {
        os << std::setw(14) << (s.currentValue_ >= 1.0 ? "UNLOCKED" : "LOCKED");
    } else {
        std::ostringstream valStream;
        valStream << std::fixed << std::setprecision(2) << s.currentValue_ << " " << s.unit_;
        os << std::setw(14) << valStream.str();
    }

    os << "| " << color << status << ansi::RESET << std::string(14, ' ') << "|";
    return os;
}

// --- Derived Sensors ---

EngineTemperatureSensor::EngineTemperatureSensor()
    : Sensor("Engine Temperature", SensorType::EngineTemp, "C", 0.0) {}

void EngineTemperatureSensor::display(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = isCritical(profile);
    std::cout << "| " << std::left << std::setw(22) << name_ << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left << std::setw(4) << unit_
              << "| " << (critical ? ansi::RED : ansi::GREEN) << (critical ? "[WARNING]" : "[NORMAL] ") << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string EngineTemperatureSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool EngineTemperatureSensor::isCritical(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ >= profile.engineThreshold;
}

BatterySensor::BatterySensor()
    : Sensor("Battery Voltage", SensorType::BatteryVoltage, "V", 0.0) {}

void BatterySensor::display(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = isCritical(profile);
    std::cout << "| " << std::left << std::setw(22) << name_ << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left << std::setw(4) << unit_
              << "| " << (critical ? ansi::RED : ansi::GREEN) << (critical ? "[WARNING]" : "[NORMAL] ") << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string BatterySensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool BatterySensor::isCritical(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ <= profile.batteryThreshold || currentValue_ >= 15.0;
}

SpeedSensor::SpeedSensor()
    : Sensor("Vehicle Speed", SensorType::VehicleSpeed, "km/h", 0.0) {}

void SpeedSensor::display(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = isCritical(profile);
    std::cout << "| " << std::left << std::setw(22) << name_ << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left << std::setw(4) << unit_
              << "| " << (critical ? ansi::YELLOW : ansi::GREEN) << (critical ? "[WARNING]" : "[NORMAL] ") << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string SpeedSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool SpeedSensor::isCritical(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ >= profile.speedLimit;
}

TirePressureSensor::TirePressureSensor()
    : Sensor("Tire Pressure", SensorType::TirePressure, "PSI", 0.0) {}

void TirePressureSensor::display(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool critical = isCritical(profile);
    std::cout << "| " << std::left << std::setw(22) << name_ << "| " << std::right << std::setw(9) << std::fixed
              << std::setprecision(2) << currentValue_ << " " << std::left << std::setw(4) << unit_
              << "| " << (critical ? ansi::YELLOW : ansi::GREEN) << (critical ? "[WARNING]" : "[NORMAL] ") << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string TirePressureSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << currentValue_ << " " << unit_;
    return oss.str();
}

bool TirePressureSensor::isCritical(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ <= profile.tireThreshold || currentValue_ >= 45.0;
}

DoorSensor::DoorSensor()
    : Sensor("Door Status", SensorType::DoorStatus, "", 0.0) {}

void DoorSensor::display(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool isOpen = isCritical(profile);
    std::cout << "| " << std::left << std::setw(22) << name_ << "| " << std::setw(14) << (isOpen ? "OPEN" : "CLOSED")
              << "| " << (isOpen ? ansi::YELLOW : ansi::GREEN) << (isOpen ? "[WARNING]" : "[NORMAL] ") << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string DoorSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return (currentValue_ >= 1.0) ? "OPEN" : "CLOSED";
}

bool DoorSensor::isCritical(const DriverProfile& /*profile*/) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ >= 1.0;
}

SeatbeltSensor::SeatbeltSensor()
    : Sensor("Seatbelt Status", SensorType::Seatbelt, "", 0.0) {}

void SeatbeltSensor::display(const DriverProfile& profile) const {
    std::lock_guard<std::mutex> lock(mtx_);
    bool isUnlocked = isCritical(profile);
    std::cout << "| " << std::left << std::setw(22) << name_ << "| " << std::setw(14) << (isUnlocked ? "UNLOCKED" : "LOCKED")
              << "| " << (isUnlocked ? ansi::YELLOW : ansi::GREEN) << (isUnlocked ? "[WARNING]" : "[NORMAL] ") << ansi::RESET
              << std::string(14, ' ') << "|" << std::endl;
}

std::string SeatbeltSensor::getFormattedValue() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return (currentValue_ >= 1.0) ? "UNLOCKED" : "LOCKED";
}

bool SeatbeltSensor::isCritical(const DriverProfile& /*profile*/) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return currentValue_ >= 1.0;
}

bool Sensor::validateInput(double /*value*/, std::string& /*warningMsg*/, std::string& /*errorMsg*/) const {
    return true;
}

bool EngineTemperatureSensor::validateInput(double value, std::string& /*warningMsg*/, std::string& errorMsg) const {
    if (value < -50.0 || value > 200.0) {
        errorMsg = "Invalid temperature. Must be between -50 C and 200 C.";
        return false;
    }
    return true;
}

bool BatterySensor::validateInput(double value, std::string& warningMsg, std::string& errorMsg) const {
    if (value < 0.0 || value > 20.0) {
        errorMsg = "Invalid voltage. Must be between 0 V and 20 V.";
        return false;
    }
    if (value < 13.5) {
        warningMsg = "Low battery voltage.";
    } else if (value > 15.0) {
        warningMsg = "High battery voltage.";
    }
    return true;
}

bool SpeedSensor::validateInput(double value, std::string& /*warningMsg*/, std::string& errorMsg) const {
    if (value < 0.0 || value > 300.0) {
        errorMsg = "Invalid speed. Must be between 0 and 300 km/h.";
        return false;
    }
    return true;
}

bool TirePressureSensor::validateInput(double value, std::string& warningMsg, std::string& errorMsg) const {
    if (value < 0.0 || value > 50.0) {
        errorMsg = "Invalid tire pressure. Must be between 0 and 50 PSI.";
        return false;
    }
    if (value > 40.0) {
        warningMsg = "High tire pressure detected.";
    }
    return true;
}

bool DoorSensor::validateInput(double value, std::string& /*warningMsg*/, std::string& errorMsg) const {
    if (value != 0.0 && value != 1.0) {
        errorMsg = "Invalid door status. Must be exactly 0 (CLOSED) or 1 (OPEN).";
        return false;
    }
    return true;
}

bool SeatbeltSensor::validateInput(double value, std::string& /*warningMsg*/, std::string& errorMsg) const {
    if (value != 0.0 && value != 1.0) {
        errorMsg = "Invalid seatbelt status. Must be exactly 0 (LOCKED) or 1 (UNLOCKED).";
        return false;
    }
    return true;
}
