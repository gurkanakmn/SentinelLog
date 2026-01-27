/**
 * @file LogLevel.h
 * @brief Severity level definitions and conversion utilities.
 */

#pragma once

#include <string>
#include <algorithm>

/**
 * @enum LogLevel
 * @brief Priority levels for log messages.
 */
enum class LogLevel {
    DEBUG = 0, /**< Detailed diagnostic information. */
    INFO,      /**< General operational messages. */
    WARNING,   /**< Non-critical issues or potential problems. */
    ERROR,     /**< Critical issues that may affect functionality. */
    FATAL      /**< Catastrophic failures requiring immediate attention. */
};

/** @brief Converts LogLevel enum to its string representation. */
inline std::string toString(LogLevel level){
    switch(level){
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::FATAL: return "FATAL";
        default: return "UNKNOWN";
    }
};

/** @brief Parses a string into a LogLevel, case-insensitive. Defaults to INFO. */
inline LogLevel toLogLevel(std::string& str){
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    if(str == "DEBUG") return LogLevel::DEBUG;
    else if(str == "INFO") return LogLevel::INFO;
    else if(str == "WARNING") return LogLevel::WARNING;
    else if(str == "ERROR") return LogLevel::ERROR;
    else if(str == "FATAL") return LogLevel::FATAL;
    return LogLevel::INFO;
};


