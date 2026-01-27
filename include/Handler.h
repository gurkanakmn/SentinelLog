/**
 * @file Handler.h
 * @brief Base class for all log output destinations.
 */

#pragma once

#include "Formatter.h"    
#include "LogRecord.h"
#include <memory>

/**
 * @class Handler
 * @brief Abstract base class for log handlers.
 * * Handlers are responsible for sending log records to specific destinations 
 * Each handler owns a Formatter to transform the LogRecord into the desired string representation.
 */
class Handler {
protected:
    std::unique_ptr<Formatter> formatter; /**< The formatting strategy used by this handler. */

public:
    /**
     * @brief Constructs a Handler with a specific formatter.
     * @param formatter Unique pointer to the formatting strategy.
     */
    explicit Handler(std::unique_ptr<Formatter> formatter);

    /** @brief Virtual destructor to ensure proper cleanup of derived classes. */
    virtual ~Handler() = default;

    /**
     * @brief Manually triggers a flush of any buffered log data.
     * Default implementation is empty; derived classes with buffers should override this.
     */
    virtual void flush();

    /**
     * @brief Pure virtual method to process a log record.
     * @param record The log entry to be handled.
     */
    virtual void handle(const LogRecord& record) = 0;
};