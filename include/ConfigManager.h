/**
 * @file ConfigManager.h
 * @brief Configuration registry for the logging framework.
 */

#pragma once

#include "Logger.h"
#include "LogLevel.h"
#include <string>
#include <map>

/**
 * @class ConfigManager
 * @brief Singleton class that parses and stores configuration settings from a file.
 * * Provides type-safe getter methods for various configuration types. 
 * Managed as a friend class of Logger to control the initialization flow.
 */
class ConfigManager {
public:
    /**
     * @brief Returns the global instance of the ConfigManager.
     * @return Reference to the Singleton instance.
     */
    static ConfigManager& getInstance();

    /**
     * @brief Retrieves a LogLevel value by key.
     * @param key The configuration key.
     * @param defaultValue Value to return if key is not found.
     * @return The parsed LogLevel or default.
     */
    LogLevel getLogLevel(const std::string& key, LogLevel defaultValue);

    /**
     * @brief Retrieves a string value by key.
     * @param key The configuration key.
     * @param defaultValue Value to return if key is not found.
     * @return The stored string or default.
     */
    std::string getString(const std::string& key, const std::string& defaultValue);

    /**
     * @brief Retrieves an integer value by key.
     * @param key The configuration key.
     * @param defaultValue Value to return if key is not found.
     * @return The parsed integer or default.
     */
    int getInt(const std::string& key, int defaultValue);

    /**
     * @brief Retrieves a boolean value by key.
     * @param key The configuration key.
     * @param defaultValue Value to return if key is not found.
     * @return The parsed boolean or default.
     */
    bool getBool(const std::string& key, bool defaultValue);

private:
    ConfigManager() = default;
    ~ConfigManager() = default;

    /** @brief Internal storage for key-value pairs parsed from the config file. */
    std::map<std::string, std::string> settings;

    /**
     * @brief Loads and parses settings from a specific file.
     * @param filename Path to the .conf file.
     * @return true if loaded successfully, false otherwise.
     */
    bool load(const std::string& filename);

    /** @brief Utility to remove leading/trailing whitespace during parsing. */
    std::string trim(const std::string& str);

    friend class Logger;
};