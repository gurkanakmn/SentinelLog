/**
 * @file CrashGuard.h
 * @brief Signal handling utility for post-crash logging.
 */

#pragma once
#include "Logger.h"
#include <csignal>

/**
 * @class CrashGuard
 * @brief Intercepts OS signals (like SIGSEGV) to perform emergency flushes.
 * * Ensures that if the application crashes, any logs remaining in the 
 * memory buffer or handler buffers are written to disk before termination.
 */
class CrashGuard {
public:
    /**
     * @brief Registers signal handlers for common crash scenarios (SIGSEGV, SIGABRT, etc.).
     */
    static void init();

private:
    /**
     * @brief Static callback executed by the OS when a signal is received.
     * @param signal The received signal identifier.
     */
    static void handleSignal(int signal);
};