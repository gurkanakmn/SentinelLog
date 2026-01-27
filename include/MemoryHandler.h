/**
 * @file MemoryHandler.h
 * @brief Circular buffer handler for incident diagnostic reports.
 */

#pragma once
#include "Handler.h"
#include <deque>
#include <memory>
#include <mutex>
#include <vector>

/**
 * @class MemoryHandler
 * @brief Keeps a fixed-size window of recent logs in memory.
 * * This handler acts as a "Black Box" recorder. It stores logs in a circular 
 * buffer and only flushes them to target handlers (like a file) when 
 * a specific LogLevel (e.g., ERROR) is triggered.
 */
class MemoryHandler : public Handler {
private:
    std::vector<std::shared_ptr<Handler>> targets; /**< Destinations for the memory dump. */
    size_t capacity;                               /**< Max number of records to keep. */
    LogLevel flushLevel;                           /**< Level that triggers a buffer dump. */
    
    std::deque<LogRecord> buffer;                  /**< Circular memory buffer. */
    std::mutex bufferMutex;                        /**< Mutex protecting the buffer. */

    /** @brief Dumps current buffer contents to all targets. */
    void flushBuffer();

public:
    /**
     * @brief Constructs a MemoryHandler.
     * @param capacity Maximum logs to store.
     * @param triggerLevel Severity that causes an automatic dump.
     */
    MemoryHandler(size_t capacity = 128, LogLevel triggerLevel = LogLevel::ERROR);

    /** @brief Adds a handler that will receive the memory dump. */
    void addTarget(std::shared_ptr<Handler> target);

    /** @brief Adds the record to the circular buffer and checks for trigger level. */
    void handle(const LogRecord& record) override;

    /** @brief Explicitly triggers a memory dump. */
    void flush() override;
};