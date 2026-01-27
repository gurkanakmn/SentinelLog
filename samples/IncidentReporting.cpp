#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "../include/Logger.h"

auto& logger = Logger::getInstance();

// waitings are for realism

void stressTest(int threadId) {
    for (int i = 0; i < 100; ++i) {
        logger.info("Thread " + std::to_string(threadId) + " is processing data index: " + std::to_string(i));
        
        if (i % 25 == 0) {
            logger.warning("Thread " + std::to_string(threadId) + " encountered a minor latency issue.");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    std::cout << "=== CrashGuard Logging Framework: Grand Test Starting ===" << std::endl;

    logger.setConfigPath("../logger.conf");
    logger.initConfig();
    // 2. Multi-threaded Test
    std::cout << "[Step 1] Launching 5 threads for concurrent logging..." << std::endl;
    std::vector<std::thread> threads;
    for (int i = 1; i <= 5; ++i) {
        threads.emplace_back(stressTest, i);
    }

    // Wait for threads to end
    for (auto& t : threads) {
        t.join();
    }

    // Log Rotation Test
    std::cout << "[Step 2] Generating heavy traffic for Log Rotation..." << std::endl;
    for (int i = 0; i < 500; ++i) {
        logger.debug("Spamming logs to trigger rotation. Current index: " + std::to_string(i));
    }

    // Incident Diagnostics (Black Box) Test
    std::cout << "[Step 3] Simulating a FATAL system failure..." << std::endl;
    logger.info("System is stable... Everything looks fine.");
    logger.info("Waiting for a potential crash scenario...");
    
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // This should trigger the MemoryHandler
    logger.fatal("CRITICAL FAILURE: Kernel heap corruption detected! Initiating core dump...");


    std::cout << "[Step 4] Test complete. Checking files..." << std::endl;
    std::cout << "Please check 'logs/' folder for 'app_main.log', '.log.1' and 'incident_report.json'" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}