/**
 * @file LogRecord.h
 * @brief Container for log entry data and metadata.
 */

#pragma once
#include <chrono>
#include <string>
#include <thread>
#include <sstream>
#include <source_location>
#include "LogLevel.h"

/**
 * @struct LogRecord
 * @brief Encapsulates a single log entry.
 * * Captures the timestamp, severity, and message, along with optional 
 * contextual metadata like thread ID and source code location.
 */
struct LogRecord {
    std::chrono::system_clock::time_point timestamp; /**< When the log was created. */
    LogLevel level;                                  /**< Severity level. */
    std::string message;                             /**< The actual log message. */

    std::string threadId;                            /**< ID of the thread that generated the log. */
    std::string fileName;                            /**< Source file name. */
    std::string functionName;                        /**< Function/Method name. */
    int lineNumber;                                  /**< Line number in the source file. */

    /**
     * @brief Factory method to create a populated LogRecord.
     * @param metadataEnabled If true, captures thread and location info.
     * @param level The severity level.
     * @param message The log message.
     * @param loc C++20 source location captured at the call site.
     * @return A fully initialized LogRecord.
     */
    static LogRecord create(bool metadataEnabled, LogLevel level, const std::string& message, const std::source_location& loc) {
    LogRecord record;
    record.timestamp = std::chrono::system_clock::now();
    record.level = level;
    record.message = message;

    if (metadataEnabled) {
        thread_local static std::string cachedThreadId = "";
        if(cachedThreadId.empty()) {
            std::stringstream ss;
            ss << std::this_thread::get_id();
            cachedThreadId = ss.str();
        }
        record.threadId = cachedThreadId;

        std::string fullPath = loc.file_name();
        size_t lastSlash = fullPath.find_last_of("/\\");
        record.fileName = (lastSlash == std::string::npos) ? fullPath : fullPath.substr(lastSlash + 1);
        record.functionName = loc.function_name();
        record.lineNumber = loc.line();
    } else {
        record.threadId = "";
        record.fileName = "";
        record.functionName = "";
        record.lineNumber = -1;
    }

    return record;
}
};