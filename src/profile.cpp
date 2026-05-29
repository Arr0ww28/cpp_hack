#include "profile.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

ProfileManager::ProfileManager(const std::string& configFilePath)
    : activeProfileIndex_(0), configFilePath_(configFilePath) {
    loadProfiles();
}

// Very simple manual JSON parser
// Expected format:
// {
//   "profiles": [
//     {
//       "name": "Default",
//       "engineThreshold": 115.0,
//       "batteryThreshold": 11.0,
//       "tireThreshold": 25.0,
//       "speedLimit": 120.0,
//       "doorSpeedThreshold": 5.0
//     }
//   ]
// }
void ProfileManager::loadProfiles() {
    std::ifstream file(configFilePath_);
    if (!file.is_open()) {
        // Create default profiles if file doesn't exist
        profiles_.push_back({"Default", 110.0, 11.0, 25.0, 120.0, 10.0});
        profiles_.push_back({"Eco", 100.0, 11.5, 30.0, 90.0, 0.0});
        profiles_.push_back({"Sport", 125.0, 10.5, 22.0, 150.0, 10.0});
        profiles_.push_back({"Performance", 135.0, 10.0, 20.0, 180.0, 15.0});
        saveProfiles();
        return;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Quick and dirty manual parsing
    size_t pos = 0;
    while ((pos = content.find("\"name\"", pos)) != std::string::npos) {
        DriverProfile p;
        
        // Parse name
        size_t startQuote = content.find("\"", pos + 6);
        if (startQuote != std::string::npos) {
            startQuote++;
            size_t endQuote = content.find("\"", startQuote);
            if (endQuote != std::string::npos) {
                p.name = content.substr(startQuote, endQuote - startQuote);
            }
        }
        
        auto parseDouble = [&](const std::string& key) -> double {
            size_t kPos = content.find("\"" + key + "\"", pos);
            if (kPos == std::string::npos) return 0.0;
            size_t colon = content.find(":", kPos);
            if (colon == std::string::npos) return 0.0;
            size_t nextComma = content.find(",", colon);
            size_t nextBrace = content.find("}", colon);
            size_t endPos = std::min(nextComma, nextBrace);
            if (endPos == std::string::npos) return 0.0;
            
            std::string valStr = content.substr(colon + 1, endPos - colon - 1);
            // remove whitespace
            valStr.erase(std::remove_if(valStr.begin(), valStr.end(), ::isspace), valStr.end());
            try { return std::stod(valStr); } catch (...) { return 0.0; }
        };

        p.engineThreshold = parseDouble("engineThreshold");
        p.batteryThreshold = parseDouble("batteryThreshold");
        p.tireThreshold = parseDouble("tireThreshold");
        p.speedLimit = parseDouble("speedLimit");
        p.doorSpeedThreshold = parseDouble("doorSpeedThreshold");

        profiles_.push_back(p);
        pos += 6;
    }

    if (profiles_.empty()) {
        profiles_.push_back({"Default", 115.0, 11.0, 25.0, 120.0, 5.0});
    }
}

void ProfileManager::saveProfiles() const {
    std::ofstream file(configFilePath_);
    if (!file.is_open()) return;

    file << "{\n  \"profiles\": [\n";
    for (size_t i = 0; i < profiles_.size(); ++i) {
        const auto& p = profiles_[i];
        file << "    {\n";
        file << "      \"name\": \"" << p.name << "\",\n";
        file << "      \"engineThreshold\": " << p.engineThreshold << ",\n";
        file << "      \"batteryThreshold\": " << p.batteryThreshold << ",\n";
        file << "      \"tireThreshold\": " << p.tireThreshold << ",\n";
        file << "      \"speedLimit\": " << p.speedLimit << ",\n";
        file << "      \"doorSpeedThreshold\": " << p.doorSpeedThreshold << "\n";
        file << "    }";
        if (i < profiles_.size() - 1) file << ",";
        file << "\n";
    }
    file << "  ]\n}\n";
}
