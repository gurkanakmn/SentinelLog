#include "../include/ConfigManager.h"
#include <fstream>
#include <algorithm>

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

bool ConfigManager::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
    size_t commentPos = line.find('#');
    if (commentPos != std::string::npos) {
        line = line.substr(0, commentPos);
    }

    line = trim(line);

    if (line.empty()) {
        continue;
    }

    size_t delimiterPos = line.find('=');
    if (delimiterPos != std::string::npos) {
        std::string key = trim(line.substr(0, delimiterPos));
        std::string value = trim(line.substr(delimiterPos + 1));
        
        if (!key.empty()) {
            settings[key] = value;
        }
    }
}
    file.close();
    return true;
}

LogLevel ConfigManager::getLogLevel(const std::string& key, LogLevel defaultValue){
    if(settings.find(key) != settings.end()){
        return toLogLevel(settings[key]);
    }
    return defaultValue;
}

std::string ConfigManager::getString(const std::string& key, const std::string& defaultValue) {
    if (settings.find(key) != settings.end()) {
        return settings[key];
    }
    return defaultValue;
}

int ConfigManager::getInt(const std::string& key, int defaultValue) {
    if (settings.find(key) != settings.end()) {
        try {
            return std::stoi(settings[key]);
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

bool ConfigManager::getBool(const std::string& key, bool defaultValue) {
    if (settings.find(key) != settings.end()) {
        std::string val = settings[key];
        std::transform(val.begin(), val.end(), val.begin(), ::tolower);
        return (val == "true" || val == "1" || val == "yes");
    }
    return defaultValue;
}

std::string ConfigManager::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (std::string::npos == first) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}