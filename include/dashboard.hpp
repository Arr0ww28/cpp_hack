/**
 * @file dashboard.hpp
 * @brief Vehicle Dashboard Module — Display & Statistics
 *
 * Provides the VehicleStatistics class (min/max/avg tracking per sensor)
 * and the Dashboard class (ASCII-only ANSI-colored CLI views with
 * menu-driven navigation).
 *
 * C++ Concepts Demonstrated:
 *   - STL Containers (std::map for statistics)
 *   - Operator Overloading (operator<< for VehicleStatistics)
 *   - Stream Operators (setw, setprecision, fixed, left, right)
 *   - Mutex / lock_guard (thread-safe statistics)
 *   - Const-correctness (render methods are const)
 */

#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include "sensor.hpp"
#include "alert.hpp"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

// =============================================================================
// VehicleStatistics — Per-Sensor Min/Max/Avg Tracking
// =============================================================================

/**
 * @class VehicleStatistics
 * @brief Tracks minimum, maximum, and running average for each sensor type.
 *
 * Thread-safe — all methods acquire the internal mutex.
 * Uses std::map for O(log n) lookup by SensorType.
 */
class VehicleStatistics {
private:
    std::map<SensorType, double> maxValues_;         ///< Highest reading per sensor
    std::map<SensorType, double> minValues_;         ///< Lowest reading per sensor
    std::map<SensorType, double> avgAccumulators_;   ///< Running sum for avg
    std::map<SensorType, int>    sampleCounts_;      ///< Number of samples
    mutable std::mutex mtx_;

public:
    VehicleStatistics() = default;
    ~VehicleStatistics() = default;

    /**
     * @brief Record a new reading for a sensor.
     *        Updates min, max, and running average.
     * @param type   SensorType to record under
     * @param value  The reading value
     */
    void recordReading(SensorType type, double value);

    /** @brief Get the maximum recorded value for a sensor type. */
    double getMax(SensorType type) const;

    /** @brief Get the minimum recorded value for a sensor type. */
    double getMin(SensorType type) const;

    /** @brief Get the running average for a sensor type. */
    double getAvg(SensorType type) const;

    /** @brief Get sample count for a sensor type. */
    int getSampleCount(SensorType type) const;

    /** @brief Check if any readings have been recorded for a sensor type. */
    bool hasData(SensorType type) const;

    /**
     * @brief Stream insertion for formatted statistics table.
     */
    friend std::ostream& operator<<(std::ostream& os, const VehicleStatistics& stats);
};

// =============================================================================
// Dashboard — CLI Rendering Engine
// =============================================================================

/**
 * @class Dashboard
 * @brief Renders ASCII-only, ANSI-colored CLI views for the monitoring system.
 *
 * Uses only: +, -, |, = for borders. Colors: green/yellow/red + bold + reset.
 * No emojis. No Unicode box-drawing characters.
 *
 * Provides multiple render methods for different menu views:
 *   - renderLiveDashboard()  — sensor table + active alerts (auto-refresh)
 *   - renderActiveAlerts()   — detailed active alert view
 *   - renderAlertHistory()   — historical alert log
 *   - renderStatistics()     — min/max/avg statistics table
 *   - renderMenu()           — main navigation menu
 */
class Dashboard {
private:
    const std::vector<std::unique_ptr<Sensor>>& sensors_;    ///< Non-owning ref
    const AlertManager&                         alertMgr_;   ///< Non-owning ref
    VehicleStatistics&                          stats_;      ///< Non-owning ref
    mutable std::mutex mtx_;

    static constexpr int TABLE_WIDTH = 62;

    // --- ANSI Helpers (green / yellow / red / bold / reset ONLY) ---
    static std::string green(const std::string& text);
    static std::string yellow(const std::string& text);
    static std::string red(const std::string& text);
    static std::string bold(const std::string& text);

    // --- Layout Helpers ---
    static void clearScreen();
    static void printSeparator(char fill = '-', int width = TABLE_WIDTH);
    static void printDoubleSeparator(int width = TABLE_WIDTH);

    void printHeader(const std::string& subtitle = "[LIVE]") const;
    void printSensorTable() const;
    void printActiveAlerts() const;
    void printAlertHistorySection(size_t count = 10) const;
    void printStatisticsTable() const;
    void printFooter(const std::string& prompt = "Press [Enter] to return to menu") const;

    /**
     * @brief Convert SensorType to display name string.
     */
    static std::string sensorTypeName(SensorType type);

public:
    /**
     * @brief Construct Dashboard with references to shared state.
     *        Dashboard does NOT own any of these objects.
     */
    Dashboard(const std::vector<std::unique_ptr<Sensor>>& sensors,
              const AlertManager& alertMgr,
              VehicleStatistics& stats);

    ~Dashboard() = default;

    // --- Render Methods (one per menu view) ---

    /** @brief Full live dashboard: header + sensors + active alerts. */
    void renderLiveDashboard() const;

    /** @brief Active alerts detail view. */
    void renderActiveAlerts() const;

    /** @brief Alert history view (last N alerts). */
    void renderAlertHistory(size_t count = 20) const;

    /** @brief Statistics summary view (min/max/avg per sensor). */
    void renderStatistics() const;

    /** @brief Main navigation menu. */
    void renderMenu() const;
};

#endif // DASHBOARD_HPP
