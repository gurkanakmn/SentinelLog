/**
 * @file BasicUsage.cpp
 * @brief Demonstrates the basic integration and logging levels.
 */

#include "../include/Logger.h"

int main() {
    // Setup
    auto& logger = Logger::getInstance();
    logger.setConfigPath("../logger.conf"); 
    logger.initConfig();

    // Start logging with different levels
    logger.info("Application started successfully.");
    logger.warning("Configuration loaded, but some optional parameters were missing.");
    logger.debug("Establishing connection to database...");
    
    // Log with custom data
    int userId = 42;
    logger.info("User with ID: " + std::to_string(userId) + " logged in.");

    return 0;
}