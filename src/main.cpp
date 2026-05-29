#include "sensor.hpp"
#include "alert.hpp"
#include "dashboard.hpp"
#include "logger.hpp"
#include "profile.hpp"
#include "health.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <csignal>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <functional>
#include <cmath>
#include <cstdlib>

namespace main_ansi {
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RED    = "\033[31m";
    const std::string BOLD   = "\033[1m";
    const std::string RESET  = "\033[0m";
}

static std::atomic<bool> g_running{true};
std::atomic<bool> g_sensorsInitialized{false};
std::atomic<bool> g_simulateHang{false};
std::atomic<bool> g_autoSimulate{false};

void signalHandler(int /*signum*/) {
    g_running.store(false);
}

std::map<std::string, std::string> loadConfig(const std::string& filePath) {
    std::map<std::string, std::string> config;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filePath);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key   = line.substr(0, pos);
            std::string value = line.substr(pos + 1);

            auto trim = [](std::string& s) {
                s.erase(0, s.find_first_not_of(" \t\r\n"));
                s.erase(s.find_last_not_of(" \t\r\n") + 1);
            };
            trim(key);
            trim(value);
            config[key] = value;
        }
    }
    return config;
}

int getConfigInt(const std::map<std::string, std::string>& config, const std::string& key, int defaultVal) {
    auto it = config.find(key);
    if (it != config.end()) {
        try { return std::stoi(it->second); } catch (const std::exception&) {}
    }
    return defaultVal;
}

double getConfigDouble(const std::map<std::string, std::string>& config, const std::string& key, double defaultVal) {
    auto it = config.find(key);
    if (it != config.end()) {
        try { return std::stod(it->second); } catch (const std::exception&) {}
    }
    return defaultVal;
}

class ThreadManager {
private:
    std::vector<std::thread> threads_;
    std::vector<std::string> threadNames_;

public:
    ThreadManager() {}

    ~ThreadManager() {
        for (size_t i = 0; i < threads_.size(); ++i) {
            if (threads_[i].joinable()) {
                threads_[i].join();
            }
        }
    }

    void start(std::function<void()> task, const std::string& name) {
        threadNames_.push_back(name);
        threads_.emplace_back(std::move(task));
    }

    void stopAll() {
        g_running.store(false);
    }
};

void manualSensorInput(std::vector<std::unique_ptr<Sensor>>& sensors, VehicleStatistics& stats) {
    std::cout << "\n" << main_ansi::BOLD << "+----------------------------------------------+\n"
              << "|            MANUAL SENSOR INPUT               |\n"
              << "+----------------------------------------------+\n" << main_ansi::RESET
              << "(Enter a value for each sensor. Press Enter to skip)\n"
              << "Type 'S' and press Enter to toggle Auto-Simulation thread\n\n";

    for (auto& sensor : sensors) {
        SensorType type = sensor->getType();
        std::string prompt;

        if (type == SensorType::DoorStatus) {
            prompt = "  " + sensor->getName() + " [0=CLOSED, 1=OPEN]: ";
        } else if (type == SensorType::Seatbelt) {
            prompt = "  " + sensor->getName() + " [0=LOCKED, 1=UNLOCKED]: ";
        } else {
            prompt = "  " + sensor->getName() + " (" + sensor->getUnit() + "): ";
        }

        while (true) {
            std::cout << prompt;
            std::string input;
            std::getline(std::cin, input);

            if (input.empty()) {
                std::cout << "    (skipped, keeping current value)\n";
                break;
            }

            if (input == "S" || input == "s") {
                bool state = g_autoSimulate.load();
                g_autoSimulate.store(!state);
                std::cout << "    " << main_ansi::GREEN << "[OK] Auto-Simulation is now " << (g_autoSimulate.load() ? "ON" : "OFF") << main_ansi::RESET << "\n";
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                return; // Return back to main menu immediately
            }

            try {
                double val = std::stod(input);
                std::string warnMsg, errMsg;
                
                if (!sensor->validateInput(val, warnMsg, errMsg)) {
                    std::cout << "    " << main_ansi::RED << "[INVALID] " << errMsg << main_ansi::RESET << " Please try again.\n";
                    continue;
                }

                sensor->setValue(val);
                if (type != SensorType::DoorStatus && type != SensorType::Seatbelt) {
                    stats.recordReading(type, val);
                }
                LOG_INFO("ManualInput", sensor->getName() + " set to " + std::to_string(val));
                std::cout << "    " << main_ansi::GREEN << "[OK] Value accepted." << main_ansi::RESET << "\n";
                
                if (!warnMsg.empty()) {
                    std::cout << "    " << main_ansi::YELLOW << "[WARNING] " << warnMsg << main_ansi::RESET << "\n";
                }
                break;
            } catch (const std::exception&) {
                std::cout << "    " << main_ansi::YELLOW << "Invalid numeric input. Please try again." << main_ansi::RESET << "\n";
            }
        }
    }

    g_sensorsInitialized.store(true);
    std::cout << "\n" << main_ansi::GREEN << "Values updated. Returning to menu..." << main_ansi::RESET << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void searchEventLog(EventLogger& logger) {
    std::cout << "\n" << main_ansi::BOLD << "+----------------------------------------------+\n"
              << "|            SEARCH EVENT LOG                  |\n"
              << "+----------------------------------------------+\n" << main_ansi::RESET
              << "\n  Filter by:\n    [1] Severity (DEBUG/INFO/WARNING/CRITICAL)\n"
              << "    [2] Keyword (search message text)\n    [3] Back to menu\n\n  Choice: ";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "3" || choice.empty()) return;

    std::vector<LogEntry> results;

    if (choice == "1") {
        std::cout << "  Enter severity (DEBUG/INFO/WARNING/CRITICAL): ";
        std::string severity;
        std::getline(std::cin, severity);
        std::transform(severity.begin(), severity.end(), severity.begin(), ::toupper);

        results = logger.searchEvents([&severity](const LogEntry& entry) {
            return LogEntry::levelToString(entry.level).find(severity) != std::string::npos;
        });
    } else if (choice == "2") {
        std::cout << "  Enter keyword: ";
        std::string keyword;
        std::getline(std::cin, keyword);
        std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::toupper);

        results = logger.searchEvents([&keyword](const LogEntry& entry) {
            std::string msgUpper = entry.message;
            std::transform(msgUpper.begin(), msgUpper.end(), msgUpper.begin(), ::toupper);
            std::string srcUpper = entry.source;
            std::transform(srcUpper.begin(), srcUpper.end(), srcUpper.begin(), ::toupper);
            return msgUpper.find(keyword) != std::string::npos || srcUpper.find(keyword) != std::string::npos;
        });
    }

    std::cout << "\n" << main_ansi::BOLD << "  Results: " << results.size() << " matching entries" << main_ansi::RESET << "\n"
              << "  +--------------------------------------------------------------+\n";

    size_t displayCount = std::min(results.size(), static_cast<size_t>(25));
    for (size_t i = 0; i < displayCount; ++i) {
        std::cout << "  | " << results[i] << "\n";
    }
    if (results.size() > displayCount) {
        std::cout << "  | ... and " << (results.size() - displayCount) << " more entries\n";
    }
    std::cout << "  +--------------------------------------------------------------+\n\n"
              << "  Press [Enter] to return to menu...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

int main() {
    std::signal(SIGINT, signalHandler);

    EventLogger logger("logs/vehicle_logs.txt");
    setGlobalLogger(&logger);
    LOG_INFO("Main", "=== Smart Cabin & Vehicle Health Monitor Starting ===");

    std::map<std::string, std::string> config;
    int    alertInterval     = 750;
    double engineThreshold   = 110.0;
    double batteryThreshold  = 11;
    double tireThreshold     = 25.0;
    double speedLimit        = 120.0;
    double doorSpeedThreshold = 10.0;
    int    maxAlertHistory   = 100;

    try {
        config = loadConfig("data/config.txt");
        alertInterval     = getConfigInt(config, "ALERT_CHECK_INTERVAL", 750);
        engineThreshold   = getConfigDouble(config, "ENGINE_TEMP_CRITICAL", 110.0);
        batteryThreshold  = getConfigDouble(config, "BATTERY_VOLTAGE_LOW", 11);
        tireThreshold     = getConfigDouble(config, "TIRE_PRESSURE_LOW", 25.0);
        speedLimit        = getConfigDouble(config, "SPEED_LIMIT", 120.0);
        doorSpeedThreshold = getConfigDouble(config, "DOOR_SPEED_THRESHOLD", 10.0);
        maxAlertHistory   = getConfigInt(config, "MAX_ALERT_HISTORY", 100);

        LOG_INFO("Main", "Configuration loaded from data/config.txt");
        std::cout << main_ansi::GREEN << "[OK] Configuration loaded from data/config.txt" << main_ansi::RESET << "\n";
    } catch (const std::runtime_error& e) {
        LOG_WARNING("Main", std::string("Config load failed: ") + e.what() + " — using defaults");
        std::cout << main_ansi::YELLOW << "[WARN] " << e.what() << " — using defaults." << main_ansi::RESET << "\n";
    }

    std::vector<std::unique_ptr<Sensor>> sensors;
    sensors.push_back(std::make_unique<EngineTemperatureSensor>());
    sensors.push_back(std::make_unique<BatterySensor>());
    sensors.push_back(std::make_unique<SpeedSensor>());
    sensors.push_back(std::make_unique<TirePressureSensor>());
    sensors.push_back(std::make_unique<DoorSensor>());
    sensors.push_back(std::make_unique<SeatbeltSensor>());

    LOG_INFO("Main", std::to_string(Sensor::getTotalSensorCount()) + " sensors initialized");
    std::cout << main_ansi::GREEN << "[OK] " << Sensor::getTotalSensorCount() << " sensors initialized." << main_ansi::RESET << "\n";

    ProfileManager profileMgr("data/profiles.json");
    AlertManager alertMgr(static_cast<size_t>(maxAlertHistory));
    alertMgr.updateThresholds(profileMgr.getActiveProfile());

    VehicleStatistics stats;
    Dashboard dashboard(sensors, alertMgr, stats, profileMgr);

    // Initial stats recording so dashboard has some initial values (instead of N/A)
    for (const auto& sensor : sensors) {
        SensorType type = sensor->getType();
        if (type != SensorType::DoorStatus && type != SensorType::Seatbelt) {
            stats.recordReading(type, sensor->getValue());
        }
    }

    LOG_INFO("Main", "AlertManager, VehicleStatistics, Dashboard initialized");
    std::cout << main_ansi::GREEN << "[OK] All modules initialized.\n\nStarting system in 2 seconds..." << main_ansi::RESET << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    ThreadManager threadMgr;

    HealthMonitor healthMonitor;

    // --- Thread 1: Alert Monitor ---
    threadMgr.start([&]() {
        LOG_INFO("MonitorThread", "Alert monitoring thread started");
        while (g_running.load()) {
            healthMonitor.ping("MonitorThread");
            
            if (!g_sensorsInitialized.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(alertInterval));
                continue;
            }
            try {
                alertMgr.evaluateConditions(sensors);
            } catch (const std::exception& e) {
                LOG_CRITICAL("MonitorThread", std::string("Exception: ") + e.what());
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(alertInterval));
        }
        LOG_INFO("MonitorThread", "Alert monitoring thread stopped");
    }, "MonitorThread");

    // --- Thread 2: Logger ---
    threadMgr.start([&]() {
        LOG_INFO("LoggerThread", "Logger processing thread started");
        while (g_running.load()) {
            if (g_simulateHang.load()) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
                g_simulateHang.store(false);
            }
            
            healthMonitor.ping("LoggerThread");
            
            try {
                logger.processPendingEvents();
            } catch (const std::exception& e) {
                std::cerr << "[LoggerThread] Exception: " << e.what() << "\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        logger.flushAll();
        LOG_INFO("LoggerThread", "Logger processing thread stopped");
    }, "LoggerThread");

    // --- Thread 3: Watchdog ---
    threadMgr.start([&]() {
        LOG_INFO("WatchdogThread", "Health monitor thread started");
        while (g_running.load()) {
            healthMonitor.checkHealth(alertMgr);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        LOG_INFO("WatchdogThread", "Health monitor thread stopped");
    }, "WatchdogThread");

    // --- Thread 4: Auto-Simulator ---
    threadMgr.start([&]() {
        LOG_INFO("SimThread", "Auto-simulation thread started (initially OFF)");
        auto startTime = std::chrono::steady_clock::now();
        
        while (g_running.load()) {
            healthMonitor.ping("SimThread");
            
            if (g_autoSimulate.load()) {
                auto now = std::chrono::steady_clock::now();
                double elapsed = std::chrono::duration<double>(now - startTime).count();
                
                for (auto& sensor : sensors) {
                    SensorType type = sensor->getType();
                    double newVal = 0.0;
                    bool skip = false;
                    
                    switch (type) {
                        case SensorType::EngineTemp:
                            // Base 85C, Amp 30C, Freq 0.05
                            newVal = 85.0 + 30.0 * std::sin(elapsed * 0.2);
                            break;
                        case SensorType::VehicleSpeed:
                            // Base 60km/h, Amp 60km/h, Freq 0.1
                            newVal = 60.0 + 60.0 * std::sin(elapsed * 0.5);
                            if (newVal < 0) newVal = 0.0; // Don't go backwards
                            break;
                        case SensorType::BatteryVoltage:
                            // Base 12.5V, Amp 2.0V, Freq 0.02
                            newVal = 12.5 + 2.0 * std::sin(elapsed * 0.2);
                            break;
                        case SensorType::TirePressure:
                            // Base 32 PSI, Amp 5 PSI, Freq 0.01
                            newVal = 32.0 + 5.0 * std::sin(elapsed * 0.2);
                            break;
                        case SensorType::DoorStatus:
                        case SensorType::Seatbelt:
                            // 5% chance to toggle state every 500ms
                            if (std::rand() % 100 < 5) {
                                newVal = (sensor->getValue() >= 1.0) ? 0.0 : 1.0;
                            } else {
                                skip = true;
                            }
                            break;
                        default:
                            skip = true;
                            break;
                    }
                    
                    if (!skip) {
                        sensor->setValue(newVal);
                        stats.recordReading(type, newVal);
                    }
                }
                g_sensorsInitialized.store(true);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Update sensors every 500ms
        }
        LOG_INFO("SimThread", "Auto-simulation thread stopped");
    }, "SimThread");

    // --- Dashboard logic moved to main thread ---

    while (g_running.load()) {
        dashboard.renderMenu();

        std::string input;
        std::getline(std::cin, input);

        if (std::cin.eof() || std::cin.fail()) {
            g_running.store(false);
            break;
        }

        if (input.empty()) continue;

        switch (input[0]) {
            case '1':
                while (true) {
                    dashboard.renderLiveDashboard();
                    dashboard.logSensorSnapshot();
                    dashboard.logAlertSnapshot();
                    std::cout << "\nPress [R] to Refresh, or [Enter] to return to menu: ";
                    std::getline(std::cin, input);
                    if (input.empty()) break;
                    if (input[0] == 'R' || input[0] == 'r') continue;
                    break;
                }
                break;
            case '2':
                dashboard.renderActiveAlerts();
                dashboard.logAlertSnapshot();
                std::getline(std::cin, input);
                break;
            case '3':
                dashboard.renderAlertHistory(20);
                std::getline(std::cin, input);
                break;
            case '4':
                dashboard.renderStatistics();
                dashboard.logStatisticsSnapshot();
                std::getline(std::cin, input);
                break;
            case '5':
                searchEventLog(logger);
                break;
            case '6':
                manualSensorInput(sensors, stats);
                break;
            case '7':
                g_running.store(false);
                break;
            case '8': {
                bool inProfileMenu = true;
                while (inProfileMenu) {
                    dashboard.renderProfileMenu();
                    std::string pInput;
                    std::cout << "Choice: ";
                    std::getline(std::cin, pInput);
                    if (pInput.empty()) continue;
                    
                    if (pInput[0] == 'B' || pInput[0] == 'b') {
                        inProfileMenu = false;
                    } else {
                        try {
                            size_t idx = std::stoul(pInput) - 1;
                            if (idx < profileMgr.getProfiles().size()) {
                                profileMgr.setActiveProfile(idx);
                                alertMgr.updateThresholds(profileMgr.getActiveProfile());
                                std::cout << main_ansi::GREEN << "  [OK] Switched active profile." << main_ansi::RESET << "\n";
                                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                            }
                        } catch (...) {}
                    }
                }
                break;
            }
            case '9':
                std::cout << main_ansi::RED << "  [DEBUG] Simulating LoggerThread hang for 10 seconds..." << main_ansi::RESET << "\n";
                g_simulateHang.store(true);
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
            default:
                std::cout << main_ansi::YELLOW << "  Invalid choice. Please enter 1-8." << main_ansi::RESET << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
                break;
        }
    }

    LOG_INFO("Main", "=== System Shutdown Initiated ===");

    g_running.store(false);
    logger.getPendingQueue().notifyAll();

    logger.processPendingEvents();

    std::cout << "\n" << main_ansi::BOLD
              << "+==============================================================+\n"
              << "|                    SYSTEM SHUTDOWN COMPLETE                   |\n"
              << "+==============================================================+\n" << main_ansi::RESET
              << "  Sensors active at shutdown : " << Sensor::getTotalSensorCount() << "\n"
              << "  Total alerts generated     : " << Alert::getTotalAlertCount() << "\n"
              << "  Total events logged        : " << logger.getTotalEventCount() << "\n"
              << "  Log file                   : logs/vehicle_logs.txt\n"
              << main_ansi::GREEN << "  All resources released. Goodbye.\n\n" << main_ansi::RESET;

    logger.flushAll();
    setGlobalLogger(nullptr);

    return 0;
}
