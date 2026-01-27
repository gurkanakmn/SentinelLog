/**
 * @file LogRotator.h
 * @brief Utility for managing log file size and archival.
 */

#pragma once
#include <string>
#include <filesystem>

/**
 * @class LogRotator
 * @brief Logic for rolling log files once they reach a size limit.
 * * Renames current log files to numbered backups (e.g., log.1, log.2) 
 * and deletes the oldest backups once the limit is reached.
 */
class LogRotator {
private:
    size_t maxFileSize; /**< Size threshold in bytes. */
    int maxBackups;     /**< Maximum number of archive files to keep. */

public:
    /**
     * @brief Constructs a rotator.
     * @param mfs Maximum file size.
     * @param mb Maximum backup count.
     */
    LogRotator(size_t mfs, int mb);

    /** @brief Checks if the current file size exceeds the rotation threshold. */
    bool needRotation(size_t currentFileSize) const;

    /** @brief Executes the file renaming and cleanup process. */
    void rotate(const std::string& filePath);
};