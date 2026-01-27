/**
 * @file FileHandler.h
 * @brief Thread-safe file output handler with rotation and buffering.
 */

#pragma once

#include "Handler.h"
#include "LogRotator.h"
#include "CsvFormatter.h"

#include <fstream>
#include <vector>

/**
 * @class FileHandler
 * @brief Handles writing log records to a file on disk.
 * * Supports automatic file rotation via LogRotator and high-performance 
 * writing through an internal memory buffer. Thread-safety is ensured 
 * by a local mutex during write and flush operations.
 */
class FileHandler : public Handler {
private:
    std::unique_ptr<LogRotator> rotator; /**< Manager for file size and backups. */
    std::mutex lock;                     /**< Ensures thread-safe access to the file and buffer. */

    std::string filePath;                /**< Target path for the log file. */
    std::ofstream file;                  /**< The underlying file stream. */

    size_t currentFileSize = 0;          /**< Tracks current size for rotation checks. */
  
    std::vector<std::string> buffer;     /**< Memory buffer to minimize disk I/O. */
    size_t flushThreshold;               /**< Max number of records before an automatic flush. */
    size_t currentBufferSize = 0;        /**< Current count of records in buffer. */
    
    /** @brief Internal helper to open the file in append mode. */
    void openFile();

    /** @brief Internal helper to write buffer contents to disk. */
    void flushBuffer();
    
public:
    /**
     * @brief Constructs a FileHandler.
     * @param filePath Path to the log file.
     * @param formatter Formatting strategy.
     * @param maxFileSize Maximum size in bytes before rotation.
     * @param maxBackups Number of old log files to keep.
     * @param flushThreshold Number of records to buffer before writing to disk.
     */
    explicit FileHandler(const std::string& filePath, std::unique_ptr<Formatter> formatter,
                        size_t maxFileSize = 10 * 1024 * 1024, int maxBackups = 5, size_t flushThreshold = 256);

    /** @brief Destructor ensures the buffer is flushed before closing. */
    ~FileHandler();

    /** @brief Buffers the record and triggers rotation/flush if necessary. */
    void handle(const LogRecord& record) override;

    /** @brief Explicitly flushes the memory buffer to the file. */
    void flush() override;   
};