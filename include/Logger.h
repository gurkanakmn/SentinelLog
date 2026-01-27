/**
 * @file Logger.h
 * @brief Main engine of the logging framework.
 */

#pragma once

#include "LogLevel.h"
#include "LogRecord.h"
#include "Handler.h"
#include "CrashGuard.h"
#include "ConfigManager.h"
#include "HandlerFactory.h"

#include <source_location>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <queue>
#include <filesystem>

/**
 * @class Logger
 * @brief Orchestrates the asynchronous logging process.
 * * Acting as a central hub, the Logger class collects log requests from various threads,
 * queues them, and dispatches them to registered handlers using a background worker thread.
 * It also manages runtime configuration updates (HotReload) via a dedicated watcher thread.
 */
class Logger {
private:
    std::mutex handlerMutex;           /**< Mutex protecting the handlers list. */
    std::mutex queueMutex;             /**< Mutex protecting the asynchronous log queue. */
    std::condition_variable cv;        /**< Signals the worker thread when new logs arrive. */
    std::thread workerThread;          /**< Background thread for I/O processing. */
    std::atomic<bool> stopWorker{false}; /**< Flag to signal the worker thread to terminate. */
    std::queue<LogRecord> logQueue;    /**< Thread-safe queue for incoming log records. */

    std::atomic<bool> hotReloadEnabled{false}; /**< Toggle for real-time config monitoring. */
    std::atomic<int> reloadIntervalSec{5};     /**< Polling interval for config file changes. */
    std::atomic<bool> stopWatcher{false};      /**< Flag to signal the watcher thread to terminate. */
    std::thread watcherThread;                 /**< Background thread monitoring logger.conf. */
    std::filesystem::file_time_type lastConfigTime; /**< Last known modification time of the config. */

    std::vector<std::shared_ptr<Handler>> handlers; /**< List of active output destinations. */
    std::string configPath = "logger.conf";         /**< Path of config file, default is same folder with main. */
    std::atomic<bool> metadataEnabled;              /**< Toggle for capturing thread_id/file/line/function info. */
    std::atomic<LogLevel> currentLevel = LogLevel::INFO; /**< Global threshold for logging. */

    /** @brief Private constructor to enforce Singleton pattern. */
    Logger();

    /** @brief Performs an immediate flush of all handlers during system failure. */
    void emergencyFlush();

    /** @brief Internal method to push a record into the queue. */
    void log(const LogRecord& level);

    /** @brief Main loop for the worker thread to process queued logs. */
    void processLogs();

    /** @brief Initializes and runs the configuration file watcher. */
    void startConfigWatcher();

public:
    friend class CrashGuard; /**< Allows CrashGuard to trigger emergency flushes. */

    Logger(const Logger&) = delete; /**< Deleted copy constructor. */
    
    /** @brief Returns the unique Singleton instance of the Logger. */
    static Logger& getInstance();
    
    /** @brief Ensures threads are joined and buffers are flushed upon destruction. */
    ~Logger();

    /**
     * @brief Manually triggers the configuration loading and handler initialization.
     * @details This method must be called explicitly by the user after setting any custom 
     * paths (via setConfigPath) and before the first log entry is made. It parses the 
     * configuration file, sets up the worker threads, and initializes all registered handlers.
     * @note If not called, the logger will remain in its default unconfigured state.
     */
    void initConfig();

    /** @brief Manually overrides the global log level. */
    void setLevel(LogLevel level);

    /**
     * @brief Updates the path where the logger searches for its configuration file.
     * @param path The new file path (e.g., "config/settings.conf").
     * @details By default, the logger looks for "logger.conf" in the working directory. 
     */
    void setConfigPath(std::string path);

    /** @brief Manually adds a new output handler. */
    void addHandler(std::shared_ptr<Handler> handler);

    /** @brief Logs a message at DEBUG level. */
    void debug(const std::string& message, std::source_location location = std::source_location::current());
    
    /** @brief Logs a message at INFO level. */
    void info(const std::string& message, std::source_location location = std::source_location::current());
    
    /** @brief Logs a message at WARNING level. */
    void warning(const std::string& message, std::source_location location = std::source_location::current());
    
    /** @brief Logs a message at ERROR level. */
    void error(const std::string& message, std::source_location location = std::source_location::current());
    
    /** @brief Logs a message at FATAL level. */
    void fatal(const std::string& message, std::source_location location = std::source_location::current());
};