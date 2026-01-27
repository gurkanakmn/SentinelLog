#include "../include/MemoryHandler.h"

MemoryHandler::MemoryHandler(size_t capacity, LogLevel triggerLevel) : 
                            Handler(nullptr), capacity(capacity), flushLevel(triggerLevel){}

void MemoryHandler::addTarget(std::shared_ptr<Handler> target){
    targets.push_back(target);
}

void MemoryHandler::handle(const LogRecord& record) {
    {
        std::lock_guard<std::mutex> lock(bufferMutex);
        
        buffer.push_back(record);

        if (buffer.size() > capacity) {
            buffer.pop_front();
        }
    }

    if (record.level >= flushLevel) {
        this->flush();
    }
}

void MemoryHandler::flushBuffer() {
    if (buffer.empty()) return;

    for (const auto& record : buffer) {
        for (auto& target : targets) {
            target->handle(record);
        }
    }

    for (auto& target : targets) {
        target->flush();
    }

    buffer.clear();
}

void MemoryHandler::flush(){
    std::lock_guard<std::mutex> lock(bufferMutex);
    this->flushBuffer();
}