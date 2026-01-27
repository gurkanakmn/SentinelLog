#include "../include/Logger.h"
#include <chrono>
#include <iostream>

Logger::Logger(){
    CrashGuard::init();
    if(hotReloadEnabled) startConfigWatcher();
    workerThread = std::thread(&Logger::processLogs, this);
}

Logger::~Logger(){
    {
        std::lock_guard<std::mutex> lock(handlerMutex);
        stopWatcher = true;
    }
    if (watcherThread.joinable()) {
        watcherThread.join(); //stop watcher thread
    }

    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopWorker = true;
    }
    cv.notify_all(); // Wake up worker thread if it's waiting for logs
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void Logger::initConfig(){
    ConfigManager& config = ConfigManager::getInstance();
    if (!config.load(configPath)) {
        std::lock_guard<std::mutex> lock(handlerMutex);
        std::cerr << "[ConfigManager][ERROR] : Config file "<< configPath << " couldn't opened" << std::endl;
        if(handlers.empty()) handlers.push_back(HandlerFactory::createConsoleHandler()); // Ensure at least the ConsoleHandler is active if config fails
        this->metadataEnabled = true;
        return;
    }

    this->hotReloadEnabled = config.getBool("HOT_RELOAD_ENABLED", false);
    this->reloadIntervalSec = config.getInt("HOT_RELOAD_INTERVAL_SEC", 5);
    this->currentLevel = config.getLogLevel("LOG_LEVEL", LogLevel::INFO);
    this->metadataEnabled = config.getBool("LOG_METADATA_ENABLED", true);

    stopWatcher = !hotReloadEnabled;
    if(stopWatcher && watcherThread.joinable()) watcherThread.join();

    auto newHandlers = HandlerFactory::createFromConfig(config); //Copy new handlers outside the lock to minimize "lock contention"
    {
        std::lock_guard<std::mutex> lock(handlerMutex);
        handlers = std::move(newHandlers);
    }
}


void Logger::processLogs(){
    while(true){
        std::unique_lock<std::mutex> lock(this->queueMutex);
        // Wait for logs or shutdown signal. Timeout allows periodic flush check.
        bool signaled = cv.wait_for(lock, std::chrono::milliseconds(1000), [this] {
            return !logQueue.empty() || stopWorker;
        });

        if(logQueue.empty() && stopWorker) break;

        if(!signaled){
            // Periodic flush of all handlers when idle to ensure no logs are stuck in buffers
            lock.unlock();
            std::lock_guard<std::mutex> handlerLock(this->handlerMutex);
            for (auto& h : handlers) h->flush();
            continue;
        }

        LogRecord record = logQueue.front();
        logQueue.pop();
        lock.unlock(); // Release queue lock before processing the log

        // Create a local copy of handlers to allow runtime config updates without blocking I/O
        std::vector<std::shared_ptr<Handler>> currentHandlers;
        {
            std::lock_guard<std::mutex> handlerLock(this->handlerMutex);
            currentHandlers = handlers; 
        }
        for(auto& handler : currentHandlers){
            handler->handle(record);
        }
    }
}

void Logger::startConfigWatcher(){
    ConfigManager& config = ConfigManager::getInstance();
    if (!hotReloadEnabled || watcherThread.joinable() || !config.load(configPath)) return;

    lastConfigTime = std::filesystem::last_write_time(configPath);

    // Dedicated thread to poll for configuration file changes
    watcherThread = std::thread([this]() {
        while (!stopWatcher) {
            std::this_thread::sleep_for(std::chrono::seconds(reloadIntervalSec.load()));
            try {
                auto currentTime = std::filesystem::last_write_time(configPath);
                if (currentTime != lastConfigTime) {
                    lastConfigTime = currentTime;
                    this->initConfig(); // Reload and re-initialize handlers on change
                }
            } catch (...) {} // Silently handle filesystem errors during polling
        }
    });
}

Logger& Logger::getInstance(){
    static Logger instance;
    return instance;
}

void Logger::emergencyFlush(){
    for(auto& handler : handlers){
        if(handler) try{handler->flush();} catch(...){}
    }
}

void Logger::setLevel(LogLevel level){
    this->currentLevel = level;
}

void Logger::setConfigPath(std::string path){
    this->configPath = path;
}

void Logger::log(const LogRecord& record){
    if(record.level < currentLevel) return;
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        logQueue.push(record);
    }
    cv.notify_one(); 
}

void Logger::addHandler(std::shared_ptr<Handler> handler){
    std::lock_guard<std::mutex> lock(this->handlerMutex);
    this->handlers.push_back(handler);
}

void Logger::debug(const std::string &message, std::source_location location){
    log(LogRecord::create(metadataEnabled, LogLevel::DEBUG, message, location));
}

void Logger::info(const std::string &message, std::source_location location){
    log(LogRecord::create(metadataEnabled, LogLevel::INFO, message, location));
}

void Logger::warning(const std::string &message, std::source_location location){
    log(LogRecord::create(metadataEnabled, LogLevel::WARNING, message, location));
}

void Logger::error(const std::string &message, std::source_location location){
    log(LogRecord::create(metadataEnabled, LogLevel::ERROR, message, location));
}

void Logger::fatal(const std::string &message, std::source_location location){
    log(LogRecord::create(metadataEnabled, LogLevel::FATAL, message, location));
}


