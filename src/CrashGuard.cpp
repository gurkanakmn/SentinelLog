#include "../include/CrashGuard.h"
#include <iostream>
#include <cstdlib>

void CrashGuard::init() {
    std::signal(SIGSEGV, handleSignal);
    std::signal(SIGABRT, handleSignal);
    std::signal(SIGFPE,  handleSignal);
    std::signal(SIGILL,  handleSignal);
}

void CrashGuard::handleSignal(int signal) {
    Logger& logger = Logger::getInstance();
    
    logger.fatal("APPLICATION CRASHED! Signal: " + std::to_string(signal));

    logger.emergencyFlush();

    std::cerr << "\n[!] Fatal Signal (" << signal << ") received. Logs dumped to disk." << std::endl;

    std::exit(signal);
}