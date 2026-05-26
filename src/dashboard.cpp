/**
 * @file dashboard.cpp
 * @brief Implementation of VehicleStatistics and Dashboard rendering.
 *
 * All output uses ASCII-only borders (+, -, |, =) with ANSI colors
 * limited to green (normal), yellow (warning), red (critical),
 * bold (headers), and reset. No emojis, no Unicode box-drawing.
 *
 * All functions include LOG_ calls for debug tracing.
 *
 * C++ Concepts Demonstrated:
 *   - STL Containers (std::map iteration)
 *   - Operator Overloading (operator<< for VehicleStatistics)
 *   - Stream Operators (setw, setprecision, fixed, left, right)
 *   - Mutex / lock_guard (thread-safe access)
 */

#include "dashboard.hpp"
#include "logger.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>
#include <limits>

// =============================================================================
// ANSI Color Codes (local to this TU)
// =============================================================================

namespace dash_ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
} // namespace dash_ansi

// =============================================================================
// VehicleStatistics Implementation
// =============================================================================

void VehicleStatistics::recordReading(SensorType type, double value) {
    std::lock_guard<std::mutex> lock(mtx_);

    // Update max
    if (maxValues_.find(type) == maxValues_.end() || value > maxValues_[type]) {
        maxValues_[type] = value;
    }

    // Update min
    if (minValues_.find(type) == minValues_.end() || value < minValues_[type]) {
        minValues_[type] = value;
    }

    // Update running average
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
    if (countIt == sampleCounts_.end() || countIt->second == 0) {
        return 0.0;
    }
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
       << "| " << std::setw(10) << "MAX"
       << "| " << std::setw(10) << "MIN"
       << "| " << std::setw(16) << "AVG"
       << dash_ansi::RESET << "|" << std::endl;

    // Helper lambda to print a row
    auto printRow = [&](SensorType type, const std::string& name, const std::string& unit) {
        auto maxIt = stats.maxValues_.find(type);
        auto minIt = stats.minValues_.find(type);
        auto countIt = stats.sampleCounts_.find(type);
        auto accumIt = stats.avgAccumulators_.find(type);

        double maxVal = (maxIt != stats.maxValues_.end()) ? maxIt->second : 0.0;
        double minVal = (minIt != stats.minValues_.end()) ? minIt->second : 0.0;
        double avgVal = (countIt != stats.sampleCounts_.end() && countIt->second > 0)
                        ? (accumIt->second / countIt->second) : 0.0;

        std::ostringstream maxStr, minStr, avgStr;
        maxStr << std::fixed << std::setprecision(1) << maxVal << " " << unit;
        minStr << std::fixed << std::setprecision(1) << minVal << " " << unit;
        avgStr << std::fixed << std::setprecision(1) << avgVal << " " << unit;

        os << "| " << std::left << std::setw(22) << name
           << "| " << std::setw(10) << maxStr.str()
           << "| " << std::setw(10) << minStr.str()
           << "| " << std::setw(16) << avgStr.str()
           << "|" << std::endl;
    };

    printRow(SensorType::EngineTemp,      "Engine Temperature", "C");
    printRow(SensorType::BatteryVoltage,  "Battery Voltage",    "V");
    printRow(SensorType::VehicleSpeed,    "Vehicle Speed",      "km/h");
    printRow(SensorType::TirePressure,    "Tire Pressure",      "PSI");

    return os;
}

// =============================================================================
// Dashboard — ANSI Helpers
// =============================================================================

std::string Dashboard::green(const std::string& text)  { return dash_ansi::GREEN  + text + dash_ansi::RESET; }
std::string Dashboard::yellow(const std::string& text)  { return dash_ansi::YELLOW + text + dash_ansi::RESET; }
std::string Dashboard::red(const std::string& text)     { return dash_ansi::RED    + text + dash_ansi::RESET; }
std::string Dashboard::bold(const std::string& text)    { return dash_ansi::BOLD   + text + dash_ansi::RESET; }

void Dashboard::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void Dashboard::printSeparator(char fill, int width) {
    std::cout << "+" << std::string(width, fill) << "+" << std::endl;
}

void Dashboard::printDoubleSeparator(int width) {
    std::cout << "+" << std::string(width, '=') << "+" << std::endl;
}

// =============================================================================
// Dashboard — Constructor
// =============================================================================

Dashboard::Dashboard(const std::vector<std::unique_ptr<Sensor>>& sensors,
                     const AlertManager& alertMgr,
                     VehicleStatistics& stats)
    : sensors_(sensors)
    , alertMgr_(alertMgr)
    , stats_(stats)
{
    LOG_INFO("Dashboard", "Dashboard initialized");
}

// =============================================================================
// Dashboard — Layout Helpers
// =============================================================================

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
    std::cout << "|" << dash_ansi::BOLD
              << std::string(padding, ' ') << title
              << std::string(TABLE_WIDTH - padding - static_cast<int>(title.size()), ' ')
              << dash_ansi::RESET << "|" << std::endl;

    // Timestamp + subtitle
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);

    std::ostringstream timeStr;
    timeStr << subtitle << "  Updated: " << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    std::string timeLabel = timeStr.str();
    int timePadding = (TABLE_WIDTH - static_cast<int>(timeLabel.size())) / 2;

    std::cout << "|" << std::string(timePadding, ' ') << timeLabel
              << std::string(TABLE_WIDTH - timePadding - static_cast<int>(timeLabel.size()), ' ')
              << "|" << std::endl;

    printDoubleSeparator();
}

void Dashboard::printSensorTable() const {
    // Table header
    std::cout << "| " << dash_ansi::BOLD
              << std::left << std::setw(22) << "SENSOR"
              << "| " << std::setw(14) << "VALUE"
              << "| " << std::setw(23) << "STATUS"
              << dash_ansi::RESET << "|" << std::endl;
    printSeparator();

    // Sensor rows (uses operator<< from sensor.cpp)
    for (const auto& sensor : sensors_) {
        std::cout << *sensor << std::endl;
    }
    printSeparator();
}

void Dashboard::printActiveAlerts() const {
    auto alerts = alertMgr_.getActiveAlerts();

    std::cout << "| " << dash_ansi::BOLD
              << "ACTIVE ALERTS (" << alerts.size() << ")"
              << std::string(TABLE_WIDTH - 20 - std::to_string(alerts.size()).size(), ' ')
              << dash_ansi::RESET << "|" << std::endl;
    printSeparator();

    if (alerts.empty()) {
        std::cout << "| " << green("No active alerts — all systems nominal")
                  << std::string(1, ' ') << "|" << std::endl;
    } else {
        for (const auto& alert : alerts) {
            std::ostringstream line;
            line << *alert;
            std::string lineStr = line.str();

            // Truncate if too long for the table
            // Note: ANSI codes add invisible chars, so we use raw message length
            std::string rawMsg = "[" + Alert::severityToString(alert->getSeverity())
                                + "] " + Alert::typeToString(alert->getType())
                                + " - " + alert->getMessage();

            if (static_cast<int>(rawMsg.size()) > TABLE_WIDTH - 4) {
                rawMsg = rawMsg.substr(0, TABLE_WIDTH - 7) + "...";
                // Re-colorize
                std::string color;
                switch (alert->getSeverity()) {
                    case AlertSeverity::CRITICAL: color = dash_ansi::RED;    break;
                    case AlertSeverity::WARNING:  color = dash_ansi::YELLOW; break;
                    case AlertSeverity::INFO:     color = dash_ansi::GREEN;  break;
                }
                std::cout << "| " << color << rawMsg << dash_ansi::RESET;
                int visibleLen = static_cast<int>(rawMsg.size());
                if (TABLE_WIDTH - 2 - visibleLen > 0) {
                    std::cout << std::string(TABLE_WIDTH - 2 - visibleLen, ' ');
                }
                std::cout << "|" << std::endl;
            } else {
                std::cout << "| " << lineStr;
                int visibleLen = static_cast<int>(rawMsg.size());
                int remaining = TABLE_WIDTH - 2 - visibleLen;
                if (remaining > 0) {
                    std::cout << std::string(remaining, ' ');
                }
                std::cout << "|" << std::endl;
            }
        }
    }
    printSeparator();
}

void Dashboard::printAlertHistorySection(size_t count) const {
    auto history = alertMgr_.getAlertHistory();

    std::cout << "| " << dash_ansi::BOLD
              << "ALERT HISTORY (Last " << count << ")"
              << std::string(TABLE_WIDTH - 24 - std::to_string(count).size(), ' ')
              << dash_ansi::RESET << "|" << std::endl;
    printSeparator();

    // Table header
    std::cout << "| " << dash_ansi::BOLD
              << std::left << std::setw(11) << "TIME"
              << "| " << std::setw(10) << "SEVERITY"
              << "| " << std::setw(37) << "ALERT"
              << dash_ansi::RESET << "|" << std::endl;
    printSeparator();

    if (history.empty()) {
        std::cout << "| " << green("No alert history recorded yet")
                  << std::string(11, ' ') << "|" << std::endl;
    } else {
        // Show last 'count' entries
        size_t start = (history.size() > count) ? (history.size() - count) : 0;
        for (size_t i = start; i < history.size(); ++i) {
            const auto& alert = history[i];

            std::string color;
            switch (alert->getSeverity()) {
                case AlertSeverity::CRITICAL: color = dash_ansi::RED;    break;
                case AlertSeverity::WARNING:  color = dash_ansi::YELLOW; break;
                case AlertSeverity::INFO:     color = dash_ansi::GREEN;  break;
            }

            std::string alertName = Alert::typeToString(alert->getType());
            if (alertName.size() > 35) alertName = alertName.substr(0, 35) + "..";

            std::cout << "| " << std::left << std::setw(11) << alert->getTimestamp()
                      << "| " << color << std::setw(10)
                      << Alert::severityToString(alert->getSeverity())
                      << dash_ansi::RESET
                      << "| " << color << std::setw(37) << alertName
                      << dash_ansi::RESET << "|" << std::endl;
        }
    }
    printSeparator();
}

void Dashboard::printStatisticsTable() const {
    std::cout << "| " << dash_ansi::BOLD
              << std::left << std::setw(22) << "SENSOR"
              << "| " << std::setw(10) << "MAX"
              << "| " << std::setw(10) << "MIN"
              << "| " << std::setw(16) << "AVG"
              << dash_ansi::RESET << "|" << std::endl;
    printSeparator();

    // Helper to print a statistics row
    auto printStatRow = [&](SensorType type, const std::string& name, const std::string& unit) {
        if (!stats_.hasData(type)) {
            std::cout << "| " << std::left << std::setw(22) << name
                      << "| " << std::setw(10) << "N/A"
                      << "| " << std::setw(10) << "N/A"
                      << "| " << std::setw(16) << "N/A"
                      << "|" << std::endl;
            return;
        }

        std::ostringstream maxStr, minStr, avgStr;
        maxStr << std::fixed << std::setprecision(1) << stats_.getMax(type) << " " << unit;
        minStr << std::fixed << std::setprecision(1) << stats_.getMin(type) << " " << unit;
        avgStr << std::fixed << std::setprecision(1) << stats_.getAvg(type) << " " << unit;

        std::cout << "| " << std::left << std::setw(22) << name
                  << "| " << std::setw(10) << maxStr.str()
                  << "| " << std::setw(10) << minStr.str()
                  << "| " << std::setw(16) << avgStr.str()
                  << "|" << std::endl;
    };

    printStatRow(SensorType::EngineTemp,     "Engine Temperature", "C");
    printStatRow(SensorType::BatteryVoltage, "Battery Voltage",    "V");
    printStatRow(SensorType::VehicleSpeed,   "Vehicle Speed",      "km/h");
    printStatRow(SensorType::TirePressure,   "Tire Pressure",      "PSI");

    printSeparator();

    // Summary line
    std::cout << "| Total Sensors: " << Sensor::getTotalSensorCount()
              << "  |  Total Alerts Generated: " << Alert::getTotalAlertCount()
              << std::string(TABLE_WIDTH - 50 - std::to_string(Alert::getTotalAlertCount()).size(), ' ')
              << "|" << std::endl;
}

void Dashboard::printFooter(const std::string& prompt) const {
    printDoubleSeparator();
    int padding = (TABLE_WIDTH - static_cast<int>(prompt.size())) / 2;
    std::cout << "|" << std::string(padding, ' ') << prompt
              << std::string(TABLE_WIDTH - padding - static_cast<int>(prompt.size()), ' ')
              << "|" << std::endl;
    printDoubleSeparator();
}

// =============================================================================
// Dashboard — Render Methods
// =============================================================================

void Dashboard::renderLiveDashboard() const {
    LOG_DEBUG("Dashboard", "renderLiveDashboard() called");
    std::lock_guard<std::mutex> lock(mtx_);

    clearScreen();
    printHeader("[LIVE]");
    printSensorTable();
    printActiveAlerts();
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderActiveAlerts() const {
    LOG_DEBUG("Dashboard", "renderActiveAlerts() called");
    std::lock_guard<std::mutex> lock(mtx_);

    clearScreen();
    printHeader("[ALERTS]");
    printActiveAlerts();
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderAlertHistory(size_t count) const {
    LOG_DEBUG("Dashboard", "renderAlertHistory() called, count=" + std::to_string(count));
    std::lock_guard<std::mutex> lock(mtx_);

    clearScreen();
    printHeader("[HISTORY]");
    printAlertHistorySection(count);
    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderStatistics() const {
    LOG_DEBUG("Dashboard", "renderStatistics() called");
    std::lock_guard<std::mutex> lock(mtx_);

    clearScreen();
    printDoubleSeparator();
    std::string title = "VEHICLE STATISTICS SUMMARY";
    int padding = (TABLE_WIDTH - static_cast<int>(title.size())) / 2;
    std::cout << "|" << dash_ansi::BOLD
              << std::string(padding, ' ') << title
              << std::string(TABLE_WIDTH - padding - static_cast<int>(title.size()), ' ')
              << dash_ansi::RESET << "|" << std::endl;
    printDoubleSeparator();

    printStatisticsTable();

    printFooter("Press [Enter] to return to menu");
}

void Dashboard::renderMenu() const {
    LOG_DEBUG("Dashboard", "renderMenu() called");
    std::lock_guard<std::mutex> lock(mtx_);

    clearScreen();
    printDoubleSeparator();

    std::string title = "SMART CABIN & VEHICLE HEALTH MONITOR";
    int padding = (TABLE_WIDTH - static_cast<int>(title.size())) / 2;
    std::cout << "|" << dash_ansi::BOLD
              << std::string(padding, ' ') << title
              << std::string(TABLE_WIDTH - padding - static_cast<int>(title.size()), ' ')
              << dash_ansi::RESET << "|" << std::endl;

    printDoubleSeparator();
    std::cout << "|" << std::string(TABLE_WIDTH, ' ') << "|" << std::endl;
    std::cout << "|   [1] Live Dashboard"
              << std::string(TABLE_WIDTH - 21 - 27, ' ')
              << "(auto-refreshing sensor view)" << "|" << std::endl;
    std::cout << "|   [2] View Active Alerts"
              << std::string(TABLE_WIDTH - 25 - 24, ' ')
              << "(current warnings/criticals)" << std::endl;
    std::cout << "|   [3] View Alert History"
              << std::string(TABLE_WIDTH - 25 - 24, ' ')
              << "(last 20 alerts with time)  |" << std::endl;
    std::cout << "|   [4] View Statistics"
              << std::string(TABLE_WIDTH - 22 - 27, ' ')
              << "(min / max / avg per sensor) |" << std::endl;
    std::cout << "|   [5] Search Event Log"
              << std::string(TABLE_WIDTH - 23 - 26, ' ')
              << "(filter by severity/keyword) " << "|" << std::endl;
    std::cout << "|   [6] Manual Sensor Input"
              << std::string(TABLE_WIDTH - 26 - 20, ' ')
              << "(set values for debug)  |" << std::endl;
    std::cout << "|   [7] Exit"
              << std::string(TABLE_WIDTH - 11 - 18, ' ')
              << "(graceful shutdown) |" << std::endl;
    std::cout << "|" << std::string(TABLE_WIDTH, ' ') << "|" << std::endl;

    printDoubleSeparator();
    std::cout << "Enter choice [1-7]: " << std::flush;
}
