#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include "sensor.hpp"
#include "alert.hpp"
#include "profile.hpp"

#include <string>
#include <vector>
#include <deque>
#include <map>
#include <memory>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

// Tracks min/max/avg and rolling history for sensors
class VehicleStatistics {
private:
    static constexpr size_t HISTORY_SIZE = 20;
    std::map<SensorType, double> maxValues_;
    std::map<SensorType, double> minValues_;
    std::map<SensorType, double> avgAccumulators_;
    std::map<SensorType, int>    sampleCounts_;
    std::map<SensorType, std::deque<double>> historyValues_;
    mutable std::mutex mtx_;

public:
    VehicleStatistics() = default;
    ~VehicleStatistics() = default;

    void recordReading(SensorType type, double value);
    double getMax(SensorType type) const;
    double getMin(SensorType type) const;
    double getAvg(SensorType type) const;
    int getSampleCount(SensorType type) const;
    bool hasData(SensorType type) const;
    std::deque<double> getHistory(SensorType type) const;

    friend std::ostream& operator<<(std::ostream& os, const VehicleStatistics& stats);
};

// Renders the CLI user interface
class Dashboard {
private:
    const std::vector<std::unique_ptr<Sensor>>& sensors_;
    const AlertManager&                         alertMgr_;
    VehicleStatistics&                          stats_;
    ProfileManager&                             profileMgr_;
    mutable std::mutex mtx_;

    static constexpr int TABLE_WIDTH = 62;

    static std::string green(const std::string& text);
    static std::string yellow(const std::string& text);
    static std::string red(const std::string& text);
    static std::string bold(const std::string& text);

    static void clearScreen();
    static void printSeparator(char fill = '-', int width = TABLE_WIDTH);
    static void printDoubleSeparator(int width = TABLE_WIDTH);

    void printHeader(const std::string& subtitle = "[LIVE]") const;
    void printSensorTable() const;
    void printActiveAlerts() const;
    void printAlertHistorySection(size_t count = 10) const;
    void printStatisticsTable() const;
    void printSparklines() const;
    void printFooter(const std::string& prompt = "Press [Enter] to return to menu") const;

    static std::string sensorTypeName(SensorType type);

public:
    Dashboard(const std::vector<std::unique_ptr<Sensor>>& sensors,
              const AlertManager& alertMgr, VehicleStatistics& stats, ProfileManager& profileMgr);
    ~Dashboard() = default;

    void renderLiveDashboard() const;
    void renderActiveAlerts() const;
    void renderAlertHistory(size_t count = 20) const;
    void renderStatistics() const;
    void renderMenu() const;
    void renderProfileMenu();
    void handleProfileEdit();
    void handleProfileCreate();
    
    // File logging — routes dashboard data through the global EventLogger
    void logSensorSnapshot() const;
    void logAlertSnapshot() const;
    void logStatisticsSnapshot() const;
};

#endif // DASHBOARD_HPP
