#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>
#include <vector>

// Represents a single driver's configuration thresholds
struct DriverProfile {
    std::string name;
    double engineThreshold;
    double batteryThreshold;
    double tireThreshold;
    double speedLimit;
    double doorSpeedThreshold;
};

// Manages loading, saving, and switching driver profiles
class ProfileManager {
private:
    std::vector<DriverProfile> profiles_;
    size_t activeProfileIndex_;
    std::string configFilePath_;

    void loadProfiles();
    void saveProfiles() const;

public:
    explicit ProfileManager(const std::string& configFilePath = "profiles.json");
    ~ProfileManager() = default;

    const std::vector<DriverProfile>& getProfiles() const { return profiles_; }
    
    const DriverProfile& getActiveProfile() const {
        return profiles_[activeProfileIndex_];
    }
    
    size_t getActiveIndex() const { return activeProfileIndex_; }
    
    void setActiveProfile(size_t index) {
        if (index < profiles_.size()) {
            activeProfileIndex_ = index;
            // Optionally could save active profile to a separate file, but for now it's runtime only
        }
    }

    void updateActiveProfile(const DriverProfile& updatedProfile) {
        if (activeProfileIndex_ < profiles_.size()) {
            profiles_[activeProfileIndex_] = updatedProfile;
            saveProfiles();
        }
    }
    
    void addProfile(const DriverProfile& newProfile) {
        profiles_.push_back(newProfile);
        saveProfiles();
        activeProfileIndex_ = profiles_.size() - 1; // Auto-switch to newly created profile
    }
};

#endif // PROFILE_HPP
