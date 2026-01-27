#include "../include/FileHandler.h"
#include <iostream>
#include <fstream>
#include <filesystem>


void FileHandler::openFile() {
    if (file.is_open()) file.close();
    bool isNewFile;

    if (std::filesystem::exists(filePath)) {
        currentFileSize = std::filesystem::file_size(filePath);
        isNewFile = std::filesystem::file_size(filePath) == 0;
    } else {
        isNewFile = true;
        currentFileSize = 0;
    }

    file.open(filePath, std::ios::app);

    if (!file.is_open()) {
        std::cerr << "[FileHandler][ERROR] : File couldn't opened: " << filePath << std::endl;
    }
    else if(isNewFile && formatter->getType() == FormatType::CSV){
        file << "Timestamp,Level,ThreadID,FileName,LineNumber,FunctionName,Message" << std::endl;
        currentFileSize = std::filesystem::file_size(filePath);
    }
}

void FileHandler::flush(){
    std::lock_guard<std::mutex> guard(this->lock);
    this->flushBuffer();
}

void FileHandler::flushBuffer(){
    if(!file.is_open() || buffer.empty()) return;
    for(const auto& log : buffer){
        if(rotator->needRotation(currentFileSize + log.length())){
            file.flush();
            file.close();
            rotator->rotate(filePath);
            openFile();
        }

        file << log;
        currentFileSize += log.length();
    }
    file.flush();

    buffer.clear();
    currentBufferSize = 0;
}

FileHandler::FileHandler(const std::string& filePath, std::unique_ptr<Formatter> formatter,
                    size_t maxFileSize, int maxBackups, size_t flushThreshold): 
                    Handler(move(formatter)), filePath(filePath), flushThreshold(flushThreshold)
{
    this->rotator = std::make_unique<LogRotator>(maxFileSize, maxBackups);
    try {
        std::filesystem::path path(filePath);
        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[FileHandler][ERROR] : Could not create log directories: " << e.what() << std::endl;
    }

    openFile();
}

FileHandler::~FileHandler(){
    std::lock_guard<std::mutex> lock(this->lock);
    flushBuffer();
    if(file.is_open()){
        file.close();
    }
}

void FileHandler::handle(const LogRecord& record){
    std::lock_guard<std::mutex> lock(this->lock);

    std::string formatted = this->formatter->format(record) + "\n";
    buffer.push_back(formatted);
    currentBufferSize += formatted.length();

    if(buffer.size() >= flushThreshold || record.level >= LogLevel::ERROR){
        this->flushBuffer();
    }
}
