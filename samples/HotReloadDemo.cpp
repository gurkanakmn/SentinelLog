/**
 * @file HotReloadDemo.cpp
 * @brief Showcase of the dynamic runtime configuration (Hot-Reload).
 */

#include "../include/Logger.h"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    // Setup
    auto& logger = Logger::getInstance();
    logger.setConfigPath("../logger.conf");
    logger.initConfig();

    std::cout << "--- Hot-Reload Demo Started ---" << std::endl;
    std::cout << "Try changing 'LOG_LEVEL' in logger.conf while this program is running!" << std::endl;

    for(int i = 0; i < 30; ++i) {
        // This will only show if LOG_LEVEL is set to DEBUG in the .conf file
        logger.debug("Iteration " + std::to_string(i) + ": Polling for work...");
        
        logger.info("Standard heartbeat message at iteration " + std::to_string(i));
        
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}