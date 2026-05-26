#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <string>
#include <mutex>
#include <atomic>
#include <iostream>
#include <sstream>
#include <iomanip>

// Sensor discriminator
enum class SensorType {
    EngineTemp, BatteryVoltage, VehicleSpeed, DoorStatus, Seatbelt, TirePressure
};

// Abstract base sensor class
class Sensor {
protected:
    std::string  name_;
    SensorType   type_;
    std::string  unit_;
    double       currentValue_;
    mutable std::mutex mtx_;

    static std::atomic<int> totalSensorCount_;

public:
    Sensor(std::string name, SensorType type, std::string unit, double defaultVal);
    virtual ~Sensor();

    virtual void display() const = 0;
    virtual std::string getFormattedValue() const = 0;
    virtual bool isCritical() const = 0;

    std::string getName() const;
    SensorType  getType() const;
    std::string getUnit() const;
    double      getValue() const;

    // Manually set the sensor reading
    void setValue(double value);

    static int getTotalSensorCount();

    friend std::ostream& operator<<(std::ostream& os, const Sensor& s);
};

// Engine temperature sensor
class EngineTemperatureSensor : public Sensor {
public:
    EngineTemperatureSensor();
    void display() const override;
    std::string getFormattedValue() const override;
    bool isCritical() const override;
};

// Battery voltage sensor
class BatterySensor : public Sensor {
public:
    BatterySensor();
    void display() const override;
    std::string getFormattedValue() const override;
    bool isCritical() const override;
};

// Vehicle speed sensor
class SpeedSensor : public Sensor {
public:
    SpeedSensor();
    void display() const override;
    std::string getFormattedValue() const override;
    bool isCritical() const override;
};

// Tire pressure sensor
class TirePressureSensor : public Sensor {
public:
    TirePressureSensor();
    void display() const override;
    std::string getFormattedValue() const override;
    bool isCritical() const override;
};

// Door status sensor
class DoorSensor : public Sensor {
public:
    DoorSensor();
    void display() const override;
    std::string getFormattedValue() const override;
    bool isCritical() const override;
};

// Seatbelt status sensor
class SeatbeltSensor : public Sensor {
public:
    SeatbeltSensor();
    void display() const override;
    std::string getFormattedValue() const override;
    bool isCritical() const override;
};

#endif // SENSOR_HPP
