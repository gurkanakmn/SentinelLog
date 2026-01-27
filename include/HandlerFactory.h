/**
 * @file HandlerFactory.h
 * @brief Factory class for dynamic creation of log handlers and formatters.
 */

#pragma once
#include <memory>
#include <string>
#include <algorithm>
#include <vector>

class ConfigManager;
#include "ConfigManager.h"
#include "Handler.h"
#include "LogLevel.h"
#include "ConsoleHandler.h"
#include "FileHandler.h"
#include "MemoryHandler.h"
#include "Formatter.h"
#include "DefaultFormatter.h"
#include "JsonFormatter.h"
#include "CsvFormatter.h"

/**
 * @class HandlerFactory
 * @brief Static factory class to manage the instantiation of handlers.
 * * Simplifies handler creation by centralizing logic for choosing formatters 
 * and configuring handler-specific parameters like rotation and buffer sizes.
 */
class HandlerFactory {
public:
    /** @brief Creates a shared instance of a ConsoleHandler. */
    static std::shared_ptr<Handler> createConsoleHandler(bool enableColors = false);

    /** @brief Creates a shared instance of a FileHandler with rotation support. */
    static std::shared_ptr<Handler> createFileHandler(const std::string& path, 
                                                    std::string& format,
                                                    size_t maxFileSize,
                                                    int maxBackups,
                                                    size_t flushThreshold);

    /** @brief Creates a shared instance of a MemoryHandler for incident reporting. */
    static std::shared_ptr<Handler> createMemoryHandler(int capacity, 
                                                    LogLevel trigger, 
                                                    const std::string& dumpPath,
                                                    std::string& format);

    /**
     * @brief Parses the provided ConfigManager to instantiate all enabled handlers.
     * @param config Reference to the loaded configuration.
     * @return A vector of shared pointers to the configured handlers.
     */
    static std::vector<std::shared_ptr<Handler>> createFromConfig(ConfigManager& config);

private:
    /** @brief Internal utility to create a specific Formatter based on a string type. */
    static std::unique_ptr<Formatter> createFormatter(std::string& formatType);
};