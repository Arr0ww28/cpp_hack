#include "dashboard.hpp"
#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <limits>

namespace dash_ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
}

void VehicleStatistics::recordReading(SensorType type, double value) {
    std::lock_guard<std::mutex> lock(mtx_);
    if (maxValues_.find(type) == maxValues_.end() || value > maxValues_[type]) maxValues_[type] = value;
    if (minValues_.find(type) == minValues_.end() || value < minValues_[type]) minValues_[type] = value;
    avgAccumulators_[type] += value;
    sampleCounts_[type]++;
}

double VehicleStatistics::getMax(SensorType type) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = maxValues_.find(type);
    return (it != maxValues_.end()) ? it->second : 0.0;
}

double VehicleStatistics::getMin(SensorType type) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = minValues_.find(type);
    return (it != minValues_.end()) ? it->second : 0.0;
}

double VehicleStatistics::getAvg(SensorType type) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto countIt = sampleCounts_.find(type);
    auto accumIt = avgAccumulators_.find(type);
    if (countIt == sampleCounts_.end() || countIt->second == 0) return 0.0;
    return accumIt->second / countIt->second;
}

int VehicleStatistics::getSampleCount(SensorType type) const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto it = sampleCounts_.find(type);
    return (it != sampleCounts_.end()) ? it->second : 0;
}

bool VehicleStatistics::hasData(SensorType type) const {
    std::lock_guard<std::mutex> lock(mtx_);
    return sampleCounts_.find(type) != sampleCounts_.end();
}

std::ostream& operator<<(std::ostream& os, const VehicleStatistics& stats) {
    std::lock_guard<std::mutex> lock(stats.mtx_);

    os << dash_ansi::BOLD << "| " << std::left << std::setw(22) << "SENSOR"
       << "| " << std::setw(10) << "MAX" << "| " << std::setw(10) << "MIN"
       << "| " << std::setw(16) << "AVG" << dash_ansi::RESET << "|\n";

    auto printRow = [&](SensorType type, const std::string& name, const std::string& unit) {
        double maxVal = stats.maxValues_.count(type) ? stats.maxValues_.at(type) : 0.0;
        double minVal = stats.minValues_.count(type) ? stats.minValues_.at(type) : 0.0;
        double avgVal = (stats.sampleCounts_.count(type) && stats.sampleCounts_.at(type) > 0)
                        ? (stats.avgAccumulators_.at(type) / stats.sampleCounts_.at(type)) : 0.0;

        std::ostringstream maxStr, minStr, avgStr;
        maxStr << std::fixed << std::setprecision(1) << maxVal << " " << unit;
        minStr << std::fixed << std::setprecision(1) << minVal << " " << unit;
        avgStr << std::fixed << std::setprecision(1) << avgVal << " " << unit;

        os << "| " << std::left << std::setw(22) << name
           << "| " << std::setw(10) << maxStr.str() << "| " << std::setw(10) << minStr.str()
           << "| " << std::setw(16) << avgStr.str() << "|\n";
    };

    printRow(SensorType::EngineTemp,      "Engine Temperature", "C");
    printRow(SensorType::BatteryVoltage,  "Battery Voltage",    "V");
    printRow(SensorType::VehicleSpeed,    "Vehicle Speed",      "km/h");
    printRow(SensorType::TirePressure,    "Tire Pressure",      "PSI");

    return os;
}

std::string Dashboard::green(const std::string& text)  { return dash_ansi::GREEN  + text + dash_ansi::RESET; }
std::string Dashboard::yellow(const std::string& text) { return dash_ansi::YELLOW + text + dash_ansi::RESET; }
std::string Dashboard::red(const std::string& text)    { return dash_ansi::RED    + text + dash_ansi::RESET; }
std::string Dashboard::bold(const std::string& text)   { return dash_ansi::BOLD   + text + dash_ansi::RESET; }

void Dashboard::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void Dashboard::printSeparator(char fill, int width) {
    std::cout << "+" << std::string(width, fill) << "+\n";
}

void Dashboard::printDoubleSeparator(int width) {
    std::cout << "+" << std::string(width, '=') << "+\n";
}

Dashboard::Dashboard(const std::vector<std::unique_ptr<Sensor>>& sensors,
                     const AlertManager& alertMgr, VehicleStatistics& stats)
    : sensors_(sensors), alertMgr_(alertMgr), stats_(stats) {}

std::string Dashboard::sensorTypeName(SensorType type) {
    switch (type) {
        case SensorType::EngineTemp:      return "Engine Temperature";
        case SensorType::BatteryVoltage:  return "Battery Voltage";
        case SensorType::VehicleSpeed:    return "Vehicle Speed";
        case SensorType::DoorStatus:      return "Door Status";
        case SensorType::Seatbelt:        return "Seatbelt Status";
        case SensorType::TirePressure:    return "Tire Pressure";
    }
    return "Unknown";
}

void Dashboard::printHeader(const std::string& subtitle) const {
    printDoubleSeparator();
    std::string title = "SMART CABIN & VEHICLE HEALTH MONITOR";
    int padding = (TABLE_WIDTH - static_cast<int>(title.size())) / 2;
    std::cout << "|" << dash_ansi::BOLD << std::string(padding, ' ') << title
              << std::string(TABLE_WIDTH - padding - static_cast<int>(title.size()), ' ')
              << dash_ansi::RESET << "|\n";

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);
    std::ostringstream timeStr;
    timeStr << subtitle << "  Updated: " << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    std::string timeLabel = timeStr.str();
    int timePadding = (TABLE_WIDTH - static_cast<int>(timeLabel.size())) / 2;

    std::cout << "|" << std::string(timePadding, ' ') << timeLabel
              << std::string(TABLE_WIDTH - timePadding - static_cast<int>(timeLabel.size()), ' ') << "|\n";
    printDoubleSeparator();
}

void Dashboard::printSensorTable() const {
    std::cout << "| " << dash_ansi::BOLD << std::left << std::setw(22) << "SENSOR"
              << "| " << std::setw(14) << "VALUE" << "| " << std::setw(23) << "STATUS" << dash_ansi::RESET << "|\n";
    printSeparator();
    for (const auto& sensor : sensors_) {
        bool hasAlert = false;
        std::string statusStr = "[NORMAL] ";
        std::string color = dash_ansi::GREEN;

        switch (sensor->getType()) {
            case SensorType::EngineTemp:     
                hasAlert = alertMgr_.isAlertActive(AlertType::ENGINE_OVERHEAT); 
                if(hasAlert) { statusStr = "[CRITICAL]"; color = dash_ansi::RED; }
                break;
            case SensorType::BatteryVoltage: 
                hasAlert = alertMgr_.isAlertActive(AlertType::LOW_BATTERY) || alertMgr_.isAlertActive(AlertType::HIGH_BATTERY); 
                if(hasAlert) { statusStr = "[WARNING]"; color = dash_ansi::YELLOW; }
                break;
            case SensorType::VehicleSpeed:   
                hasAlert = alertMgr_.isAlertActive(AlertType::OVERSPEED); 
                if(hasAlert) { statusStr = "[WARNING]"; color = dash_ansi::YELLOW; }
                break;
            case SensorType::TirePressure:   
                hasAlert = alertMgr_.isAlertActive(AlertType::LOW_TIRE_PRESSURE) || alertMgr_.isAlertActive(AlertType::HIGH_TIRE_PRESSURE); 
                if(hasAlert) { statusStr = "[WARNING]"; color = dash_ansi::YELLOW; }
                break;
            case SensorType::DoorStatus:     
                hasAlert = alertMgr_.isAlertActive(AlertType::DOOR_OPEN); 
                if(hasAlert) { statusStr = "[CRITICAL]"; color = dash_ansi::RED; }
                break;
            case SensorType::Seatbelt:       
                hasAlert = alertMgr_.isAlertActive(AlertType::SEATBELT_WARNING); 
                if(hasAlert) { statusStr = "[WARNING]"; color = dash_ansi::YELLOW; }
                break;
        }

        std::cout << "| " << std::left << std::setw(22) << sensor->getName()
                  << "| " << std::setw(14) << sensor->getFormattedValue()
                  << "| " << color << statusStr << dash_ansi::RESET
                  << std::string(14 - (statusStr.length() - 9), ' ') << "|\n";
    }
    printSeparator();
}

void Dashboard::printActiveAlerts() const {
    auto alerts = alertMgr_.getActiveAlerts();
    std::cout << "| " << dash_ansi::BOLD << "ACTIVE ALERTS (" << alerts.size() << ")"
              << std::string(TABLE_WIDTH - 20 - std::to_string(alerts.size()).size(), ' ') << dash_ansi::RESET << "|\n";
    printSeparator();

    if (alerts.empty()) {
        std::cout << "| " << green("No active alerts — all systems nominal") << std::string(1, ' ') << "|\n";
    } else {
        for (const auto& alert : alerts) {
            std::string rawMsg = "[" + Alert::severityToString(alert->getSeverity()) + "] " + Alert::typeToString(alert->getType()) + " - " + alert->getMessage();
            std::string color = (alert->getSeverity() == AlertSeverity::CRITICAL) ? dash_ansi::RED : (alert->getSeverity() == AlertSeverity::WARNING) ? dash_ansi::YELLOW : dash_ansi::GREEN;

            size_t maxLen = TABLE_WIDTH - 4;
            size_t pos = 0;
            
            while (pos < rawMsg.size()) {
                std::string chunk = rawMsg.substr(pos, maxLen);
                std::cout << "| " << color << chunk << dash_ansi::RESET;
                int remaining = TABLE_WIDTH - 2 - chunk.size();
                if (remaining > 0) std::cout << std::string(remaining, ' ');
                std::cout << "|\n";
                pos += maxLen;
            }
        }
    }
    printSeparator();
}

void Dashboard::printAlertHistorySection(size_t count) const {
    auto history = alertMgr_.getAlertHistory();
    std::cout << "| " << dash_ansi::BOLD << "ALERT HISTORY (Last " << count << ")"
              << std::string(TABLE_WIDTH - 24 - std::to_string(count).size(), ' ') << dash_ansi::RESET << "|\n";
    printSeparator();
    std::cout << "| " << dash_ansi::BOLD << std::left << std::setw(11) << "TIME"
              << "| " << std::setw(10) << "SEVERITY" << "| " << std::setw(37) << "ALERT" << dash_ansi::RESET << "|\n";
    printSeparator();

    if (history.empty()) {
        std::cout << "| " << green("No alert history recorded yet") << std::string(11, ' ') << "|\n";
    } else {
        size_t start = (history.size() > count) ? (history.size() - count) : 0;
        for (size_t i = start; i < history.size(); ++i) {
            const auto& alert = history[i];
            std::string color = (alert->getSeverity() == AlertSeverity::CRITICAL) ? dash_ansi::RED : (alert->getSeverity() == AlertSeverity::WARNING) ? dash_ansi::YELLOW : dash_ansi::GREEN;
            std::string alertName = Alert::typeToString(alert->getType());
            if (alertName.size() > 35) alertName = alertName.substr(0, 35) + "..";
            std::cout << "| " << std::left << std::setw(11) << alert->getTimestamp()
                      << "| " << color << std::setw(10) << Alert::severityToString(alert->getSeverity()) << dash_ansi::RESET
                      << "| " << color << std::setw(37) << alertName << dash_ansi::RESET << "|\n";
        }
    }
    printSeparator();
}

void Dashboard::printStatisticsTable() const {
    std::cout << "| " << dash_ansi::BOLD << std::left << std::setw(22) << "SENSOR"
              << "| " << std::setw(10) << "MAX" << "| " << std::setw(10) << "MIN"
              << "| " << std::setw(16) << "AVG" << dash_ansi::RESET << "|\n";
    printSeparator();

    auto printStatRow = [&](SensorType type, const std::string& name, const std::string& unit) {
        if (!stats_.hasData(type)) {
            std::cout << "| " << std::left << std::setw(22) << name << "| " << std::setw(10) << "N/A"
                      << "| " << std::setw(10) << "N/A" << "| " << std::setw(16) << "N/A" << "|\n";
            return;
        }
        std::ostringstream maxStr, minStr, avgStr;
        maxStr << std::fixed << std::setprecision(1) << stats_.getMax(type) << " " << unit;
        minStr << std::fixed << std::setprecision(1) << stats_.getMin(type) << " " << unit;
        avgStr << std::fixed << std::setprecision(1) << stats_.getAvg(type) << " " << unit;

        std::cout << "| " << std::left << std::setw(22) << name << "| " << std::setw(10) << maxStr.str()
                  << "| " << std::setw(10) << minStr.str() << "| " << std::setw(16) << avgStr.str() << "|\n";
    };

    printStatRow(SensorType::EngineTemp,     "Engine Temperature", "C");
    printStatRow(SensorType::BatteryVoltage, "Battery Voltage",    "V");
    printStatRow(SensorType::VehicleSpeed,   "Vehicle Speed",      "km/h");
    printStatRow(SensorType::TirePressure,   "Tire Pressure",      "PSI");
    printSeparator();
    std::cout << "| Total Sensors: " << Sensor::getTotalSensorCount() << "  |  Total Alerts Generated: " << Alert::getTotalAlertCount()
              << std::string(TABLE_WIDTH - 50 - std::to_string(Alert::getTotalAlertCount()).size(), ' ') << "|\n";
}

void Dashboard::printFooter(const std::string& prompt) const {
    printDoubleSeparator();
    int padding = (TABLE_WIDTH - static_cast<int>(prompt.size())) / 2;
    std::cout << "|" << std::string(padding, ' ') << prompt
              << std::string(TABLE_WIDTH - padding - static_cast<int>(prompt.size()), ' ') << "|\n";
    printDoubleSeparator();
}

void Dashboard::renderLiveDashboard() const {
    std::lock_guard<std::mutex> lock(mtx_);
    clearScreen();
    printHeader("[LIVE]");
    printSensorTable();
    printActiveAlerts();
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderActiveAlerts() const {
    std::lock_guard<std::mutex> lock(mtx_);
    clearScreen();
    printHeader("[ALERTS]");
    printActiveAlerts();
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderAlertHistory(size_t count) const {
    std::lock_guard<std::mutex> lock(mtx_);
    clearScreen();
    printHeader("[HISTORY]");
    printAlertHistorySection(count);
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderStatistics() const {
    std::lock_guard<std::mutex> lock(mtx_);
    clearScreen();
    printDoubleSeparator();
    std::string title = "VEHICLE STATISTICS SUMMARY";
    int padding = (TABLE_WIDTH - static_cast<int>(title.size())) / 2;
    std::cout << "|" << dash_ansi::BOLD << std::string(padding, ' ') << title
              << std::string(TABLE_WIDTH - padding - static_cast<int>(title.size()), ' ') << dash_ansi::RESET << "|\n";
    printDoubleSeparator();
    printStatisticsTable();
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderMenu() const {
    std::lock_guard<std::mutex> lock(mtx_);
    clearScreen();
    printDoubleSeparator();
    std::string title = "SMART CABIN & VEHICLE HEALTH MONITOR";
    int padding = (TABLE_WIDTH - static_cast<int>(title.size())) / 2;
    std::cout << "|" << dash_ansi::BOLD << std::string(padding, ' ') << title
              << std::string(TABLE_WIDTH - padding - static_cast<int>(title.size()), ' ') << dash_ansi::RESET << "|\n";
    printDoubleSeparator();
    std::cout << "|" << std::string(TABLE_WIDTH, ' ') << "|\n"
              << "|   [1] Live Dashboard      (auto-refreshing sensor view)      |\n"
              << "|   [2] View Active Alerts     (current warnings/criticals     |\n"
              << "|   [3] View Alert History     (last 20 alerts with time)      |\n"
              << "|   [4] View Statistics     (min / max / avg per sensor)       |\n"
              << "|   [5] Search Event Log     (filter by severity/keyword)      |\n"
              << "|   [6] Manual Sensor Input        (set values for debug)      |\n"
              << "|   [7] Exit                         (graceful shutdown)       |\n"
              << "|" << std::string(TABLE_WIDTH, ' ') << "|\n";
    printDoubleSeparator();
    std::cout << "Enter choice [1-7]: " << std::flush;
}

void Dashboard::logSensorSnapshot() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << "--- SENSOR SNAPSHOT ---";
    for (const auto& sensor : sensors_) {
        bool hasAlert = false;
        switch (sensor->getType()) {
            case SensorType::EngineTemp:     hasAlert = alertMgr_.isAlertActive(AlertType::ENGINE_OVERHEAT); break;
            case SensorType::BatteryVoltage: hasAlert = alertMgr_.isAlertActive(AlertType::LOW_BATTERY) || alertMgr_.isAlertActive(AlertType::HIGH_BATTERY); break;
            case SensorType::VehicleSpeed:   hasAlert = alertMgr_.isAlertActive(AlertType::OVERSPEED); break;
            case SensorType::TirePressure:   hasAlert = alertMgr_.isAlertActive(AlertType::LOW_TIRE_PRESSURE) || alertMgr_.isAlertActive(AlertType::HIGH_TIRE_PRESSURE); break;
            case SensorType::DoorStatus:     hasAlert = alertMgr_.isAlertActive(AlertType::DOOR_OPEN); break;
            case SensorType::Seatbelt:       hasAlert = alertMgr_.isAlertActive(AlertType::SEATBELT_WARNING); break;
        }
        oss << " | " << sensor->getName() << ": " << sensor->getFormattedValue();
        if (hasAlert) oss << " [!]";
    }
    LOG_INFO("Dashboard", oss.str());
}

void Dashboard::logAlertSnapshot() const {
    std::lock_guard<std::mutex> lock(mtx_);
    auto alerts = alertMgr_.getActiveAlerts();
    std::ostringstream oss;
    oss << "--- ALERT SNAPSHOT --- Active: " << alerts.size();
    for (const auto& alert : alerts) {
        oss << " | [" << Alert::severityToString(alert->getSeverity()) << "] "
            << Alert::typeToString(alert->getType()) << " - " << alert->getMessage();
    }
    if (alerts.empty()) oss << " | All systems nominal";
    LOG_INFO("Dashboard", oss.str());
}

void Dashboard::logStatisticsSnapshot() const {
    std::lock_guard<std::mutex> lock(mtx_);
    std::ostringstream oss;
    oss << "--- STATISTICS SNAPSHOT ---\n" << stats_;
    LOG_INFO("Dashboard", oss.str());
}
