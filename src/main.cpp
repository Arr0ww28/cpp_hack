/**
 * @file main.cpp
 * @brief Module 1 Test Harness — Sensor Simulation Framework
 *
 * Demonstrates the sensor hierarchy, RNG-based simulation, manual input
 * mode toggle (read from config.txt), and ANSI-colored CLI display.
 *
 * This is a temporary test driver for Module 1 only. It will be replaced
 * by the full threading/menu orchestration in Module 5.
 *
 * Usage:
 *   ./vehicle_monitor
 *
 * Config:
 *   Set MANUAL_INPUT_MODE=1 in data/config.txt to manually enter values.
 *   Set MANUAL_INPUT_MODE=0 for automatic RNG simulation.
 */

#include "sensor.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <thread>
#include <atomic>
#include <chrono>
#include <csignal>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

// =============================================================================
// ANSI Color Constants
// =============================================================================

namespace ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
} // namespace ansi

// =============================================================================
// Global Shutdown Flag
// =============================================================================

static std::atomic<bool> g_running{true};

/**
 * @brief SIGINT handler for graceful shutdown (Ctrl+C).
 */
void signalHandler(int /*signum*/) {
    g_running.store(false);
}

// =============================================================================
// Config Loader
// =============================================================================

/**
 * @brief Load KEY=VALUE pairs from a configuration file.
 * @param filePath  Path to the config file.
 * @return Map of key-value string pairs.
 * @throws std::runtime_error if the file cannot be opened.
 *
 * Skips blank lines and lines starting with '#'.
 * Uses std::ifstream with RAII — file is closed automatically.
 */
std::map<std::string, std::string> loadConfig(const std::string& filePath) {
    std::map<std::string, std::string> config;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // Find the '=' delimiter
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key   = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            // Trim whitespace from key and value
            auto trim = [](std::string& s) {
                s.erase(0, s.find_first_not_of(" \t\r\n"));
                s.erase(s.find_last_not_of(" \t\r\n") + 1);
            };
            trim(key);
            trim(value);

            config[key] = value;
        }
    }

    return config;  // file closed automatically (RAII)
}

/**
 * @brief Safely get an integer from config with a default fallback.
 */
int getConfigInt(const std::map<std::string, std::string>& config,
                 const std::string& key, int defaultVal) {
    auto it = config.find(key);
    if (it != config.end()) {
        try {
            return std::stoi(it->second);
        } catch (const std::exception&) {
            // Fall through to default
        }
    }
    return defaultVal;
}

// =============================================================================
// Display Helpers
// =============================================================================

/**
 * @brief Clear the console using ANSI escape sequences.
 */
void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

/**
 * @brief Print a horizontal separator line.
 */
void printSeparator(char fill = '-', int width = 62) {
    std::cout << "+" << std::string(width, fill) << "+" << std::endl;
}

/**
 * @brief Print the dashboard header.
 */
void printHeader() {
    printSeparator('=');
    std::cout << "|" << ansi::BOLD
              << std::string(10, ' ')
              << "SMART CABIN & VEHICLE HEALTH MONITOR"
              << std::string(16, ' ')
              << ansi::RESET << "|" << std::endl;

    // Timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf{};
    localtime_r(&t, &tm_buf);

    std::ostringstream timeStr;
    timeStr << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");

    std::cout << "|" << std::string(14, ' ')
              << "[LIVE]  Updated: " << timeStr.str()
              << std::string(7, ' ') << "|" << std::endl;
    printSeparator('=');
}

/**
 * @brief Print the sensor table header row.
 */
void printTableHeader() {
    std::cout << "| " << ansi::BOLD
              << std::left << std::setw(22) << "SENSOR"
              << "| " << std::setw(14) << "VALUE"
              << "| " << std::setw(23) << "STATUS"
              << ansi::RESET << "|" << std::endl;
    printSeparator();
}

/**
 * @brief Render all sensors in a formatted table.
 */
void renderSensorTable(const std::vector<std::unique_ptr<Sensor>>& sensors) {
    clearScreen();
    printHeader();
    printTableHeader();

    for (const auto& sensor : sensors) {
        // Uses operator<< which handles colors and formatting
        std::cout << *sensor << std::endl;
    }

    printSeparator('=');
}

// =============================================================================
// Manual Input Mode
// =============================================================================

/**
 * @brief Prompt user to enter values for each sensor manually.
 * @param sensors  The sensor vector (modified in-place via setManualValue).
 *
 * For discrete sensors (Door, Seatbelt), user enters 0 or 1.
 * For continuous sensors, user enters a numeric value.
 */
void manualInputRound(std::vector<std::unique_ptr<Sensor>>& sensors) {
    std::cout << std::endl;
    std::cout << ansi::BOLD << "--- MANUAL SENSOR INPUT ---" << ansi::RESET << std::endl;
    std::cout << "(Enter a value for each sensor. Type 'q' to exit.)" << std::endl;
    std::cout << std::endl;

    for (auto& sensor : sensors) {
        SensorType type = sensor->getType();

        // Show appropriate prompt based on sensor type
        if (type == SensorType::DoorStatus) {
            std::cout << "  " << sensor->getName()
                      << " [0=CLOSED, 1=OPEN]: ";
        } else if (type == SensorType::Seatbelt) {
            std::cout << "  " << sensor->getName()
                      << " [0=LOCKED, 1=UNLOCKED]: ";
        } else {
            std::cout << "  " << sensor->getName()
                      << " (" << sensor->getUnit() << "): ";
        }

        std::string input;
        std::getline(std::cin, input);

        // Check for quit command
        if (input == "q" || input == "Q") {
            g_running.store(false);
            return;
        }

        // Parse and set value
        try {
            double val = std::stod(input);
            sensor->setManualValue(val);
        } catch (const std::exception&) {
            std::cout << "    " << ansi::YELLOW
                      << "Invalid input, keeping previous value."
                      << ansi::RESET << std::endl;
        }
    }
}

// =============================================================================
// Main Entry Point — Module 1 Test Harness
// =============================================================================

int main() {
    // Install signal handler for graceful Ctrl+C shutdown
    std::signal(SIGINT, signalHandler);

    // --- Load Configuration ---
    std::map<std::string, std::string> config;
    bool manualMode = false;
    int updateInterval = 500;  // default ms

    try {
        config = loadConfig("data/config.txt");
        manualMode     = (getConfigInt(config, "MANUAL_INPUT_MODE", 0) == 1);
        updateInterval = getConfigInt(config, "SENSOR_UPDATE_INTERVAL", 500);

        std::cout << ansi::GREEN << "[OK] Configuration loaded from data/config.txt"
                  << ansi::RESET << std::endl;
    } catch (const std::runtime_error& e) {
        std::cout << ansi::YELLOW << "[WARN] " << e.what()
                  << " — using defaults." << ansi::RESET << std::endl;
    }

    std::cout << "  Input Mode : "
              << (manualMode ? "MANUAL (user input)" : "SIMULATION (RNG)")
              << std::endl;
    std::cout << "  Update Rate: " << updateInterval << " ms" << std::endl;
    std::cout << std::endl;

    // --- Instantiate Sensors (std::unique_ptr for RAII ownership) ---
    std::vector<std::unique_ptr<Sensor>> sensors;
    sensors.push_back(std::make_unique<EngineTemperatureSensor>());
    sensors.push_back(std::make_unique<BatterySensor>());
    sensors.push_back(std::make_unique<SpeedSensor>());
    sensors.push_back(std::make_unique<TirePressureSensor>());
    sensors.push_back(std::make_unique<DoorSensor>());
    sensors.push_back(std::make_unique<SeatbeltSensor>());

    std::cout << ansi::GREEN << "[OK] " << Sensor::getTotalSensorCount()
              << " sensors initialized." << ansi::RESET << std::endl;

    // Set manual mode on all sensors if configured
    if (manualMode) {
        for (auto& s : sensors) {
            s->setManualMode(true);
        }
        std::cout << ansi::YELLOW
                  << "[INFO] Manual mode active. You will be prompted for values."
                  << ansi::RESET << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Press Ctrl+C to exit." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // --- Main Loop ---
    if (manualMode) {
        // ==============================
        // MANUAL INPUT MODE
        // ==============================
        // User enters values, then sees the dashboard.
        // Loop until user types 'q' or presses Ctrl+C.
        while (g_running.load()) {
            manualInputRound(sensors);
            if (!g_running.load()) break;

            // Render the dashboard with user-supplied values
            renderSensorTable(sensors);

            std::cout << std::endl;
            std::cout << "Press [Enter] for next input round, or 'q' to quit: ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "q" || input == "Q") {
                g_running.store(false);
            }
        }
    } else {
        // ==============================
        // SIMULATION MODE (RNG)
        // ==============================
        // Sensor update thread runs in the background at configurable interval.
        // Main thread refreshes the display every 1 second.

        std::atomic<bool> updateRunning{true};

        // Sensor update thread
        std::thread sensorThread([&]() {
            while (updateRunning.load()) {
                for (auto& sensor : sensors) {
                    sensor->update();
                }
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(updateInterval));
            }
        });

        // Main thread — display loop
        while (g_running.load()) {
            renderSensorTable(sensors);

            std::cout << "| " << ansi::BOLD
                      << "Total Sensors: " << Sensor::getTotalSensorCount()
                      << "  |  Mode: SIMULATION (RNG)"
                      << std::string(18, ' ')
                      << ansi::RESET << "|" << std::endl;
            printSeparator('=');
            std::cout << "| Press Ctrl+C to exit."
                      << std::string(40, ' ') << "|" << std::endl;
            printSeparator('=');

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        // Graceful thread shutdown
        updateRunning.store(false);
        if (sensorThread.joinable()) {
            sensorThread.join();
        }
    }

    // --- Shutdown Summary ---
    std::cout << std::endl;
    std::cout << ansi::BOLD
              << "+==============================================================+"
              << std::endl;
    std::cout << "|                    SYSTEM SHUTDOWN COMPLETE                   |"
              << std::endl;
    std::cout << "+==============================================================+"
              << ansi::RESET << std::endl;
    std::cout << "  Sensors active at shutdown: " << Sensor::getTotalSensorCount()
              << std::endl;
    std::cout << "  Mode: "
              << (manualMode ? "MANUAL" : "SIMULATION") << std::endl;
    std::cout << ansi::GREEN << "  All resources released. Goodbye."
              << ansi::RESET << std::endl;
    std::cout << std::endl;

    return 0;
}
