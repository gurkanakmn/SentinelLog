/**
 * @file Formatter.h
 * @brief Base interface for log formatting strategies.
 */

#pragma once

#include "LogRecord.h"
#include <string>
#include <chrono>

/**
 * @enum FormatType
 * @brief Supported output formats for log entries.
 */
enum class FormatType {
    TEXT,   /**< Human-readable plain text. */
    CSV,    /**< Comma-separated values for spreadsheets. */
    JSON    /**< Structured JSON format for machine parsing. */
};

/**
 * @class Formatter
 * @brief Abstract base class for all log formatters.
 * * Defines how a LogRecord is converted into a string. Provides helper 
 * methods for timestamp formatting used by derived classes.
 */
class Formatter {
public:
    Formatter() = default;
    virtual ~Formatter() = default;

    /**
     * @brief Pure virtual method to format a log record.
     * @param record The log entry to format.
     * @return Formatted string representation.
     */
    virtual std::string format(const LogRecord& record) = 0;

    /** @brief Returns the enum type of the formatter. */
    virtual FormatType getType() = 0;
    
protected:
    /** @brief Utility to format time points into local time strings. */
    std::string formatTimestampLocal(const std::chrono::system_clock::time_point& tp) const;

    /** @brief Utility to format time points into UTC time strings. */
    std::string formatTimestampUTC(const std::chrono::system_clock::time_point& tp) const;
};