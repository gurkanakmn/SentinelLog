#include "../include/HandlerFactory.h"


std::unique_ptr<Formatter> HandlerFactory::createFormatter(std::string& formatType){
    std::transform(formatType.begin(), formatType.end(), formatType.begin(), ::toupper);

    if(formatType == "CSV"){
        return std::make_unique<CsvFormatter>();
    }
    else if(formatType == "JSON"){
        return std::make_unique<JsonFormatter>();
    }
    return std::make_unique<DefaultFormatter>();
}


std::shared_ptr<Handler> HandlerFactory::createConsoleHandler(bool enableColor) {
    return std::make_shared<ConsoleHandler>(std::make_unique<DefaultFormatter>(), enableColor);
}

std::shared_ptr<Handler> HandlerFactory::createFileHandler(const std::string& path, std::string& format, 
                                                    size_t maxFileSize, int maxBackups, size_t flushThreshold)
{
    std::unique_ptr<Formatter> formatter = createFormatter(format);
    
    return std::make_shared<FileHandler>(path, move(formatter), 
                                        maxFileSize, maxBackups, flushThreshold);
}

std::shared_ptr<Handler> HandlerFactory::createMemoryHandler(int capacity, LogLevel trigger, 
                                                            const std::string& dumpPath, std::string& format) 
{
    std::unique_ptr<Formatter> formatter = createFormatter(format);
    
    auto memHandler = std::make_shared<MemoryHandler>(capacity, trigger);
    auto privateDumpFile = std::make_shared<FileHandler>(dumpPath, move(formatter), 0, 0, 1);

    memHandler->addTarget(privateDumpFile);

    return memHandler;
}

std::vector<std::shared_ptr<Handler>> HandlerFactory::createFromConfig(ConfigManager& config){
    std::vector<std::shared_ptr<Handler>> handlers;
    if (config.getBool("CONSOLE_HANDLER_ENABLED", true)) {
        handlers.push_back(HandlerFactory::createConsoleHandler(config.getBool("COLOR_ENABLED", false)));
    }

    if (config.getBool("FILE_HANDLER_ENABLED", false)) {
        std::string path = config.getString("FILE_PATH", "../logs/app.log");
        std::string format = config.getString("FILE_FORMAT", "TEXT");
        
        bool rotationActive = config.getBool("FILE_ROTATION_ENABLED", true);
        size_t maxSize = rotationActive ? config.getInt("FILE_MAX_SIZE", 10 * 1024 * 1024) : 0;
        int maxBackups = config.getInt("FILE_MAX_BACKUPS", 5);
        int flushThreshold = config.getInt("FILE_FLUSH_THRESHOLD", 100);

        handlers.push_back(HandlerFactory::createFileHandler(
            path, format, maxSize, maxBackups, flushThreshold
        ));
    }

    if (config.getBool("MEMORY_HANDLER_ENABLED", false)) {
        int capacity = config.getInt("MEMORY_CAPACITY", 250);
        std::string dumpPath = config.getString("MEMORY_DUMP_PATH", "../logs/incident.log");
        std::string format = config.getString("MEMORY_FORMAT", "TEXT");
        LogLevel trigger = config.getLogLevel("MEMORY_TRIGGER_LEVEL", LogLevel::ERROR);

        handlers.push_back(HandlerFactory::createMemoryHandler(
            capacity, trigger, dumpPath, format
        ));
    }
    return handlers;
}