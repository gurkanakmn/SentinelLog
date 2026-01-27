/**
 * @file ConsoleHandler.h
 * @brief Log handler for terminal output with color support.
 */

#pragma once

#include "Handler.h"

/** @namespace Color @brief ANSI Escape codes for colored terminal output. */
namespace Color {
    const char* const RESET  = "\033[0m";
    const char* const RED    = "\033[31m";
    const char* const GREEN  = "\033[32m";
    const char* const YELLOW = "\033[33m";
    const char* const CYAN   = "\033[36m";
    const char* const BOLD_RED   = "\033[1;31m";
}

/**
 * @class ConsoleHandler
 * @brief Responsible for printing logs to the standard output (stdout).
 * * Supports color-coded levels and ensures that terminal output does not
 * interleave log messages from multiple threads using a local mutex.
 */
class ConsoleHandler : public Handler {
private:
    std::mutex lock;     /**< Mutex to prevent console message interleaving. */
    bool enableColor;    /**< Flag to toggle ANSI color output. */

public:
    /**
     * @brief Constructs a ConsoleHandler.
     * @param formatter The formatting strategy to use.
     * @param enableColor If true, use ANSI codes for level coloring.
     */
    explicit ConsoleHandler(std::unique_ptr<Formatter> formatter, bool enableColor);

    /**
     * @brief Implements the handling logic to write the record to the terminal.
     * @param record The log entry to process.
     */
    void handle(const LogRecord& record) override;
};