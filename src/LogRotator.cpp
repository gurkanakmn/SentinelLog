#include "../include/LogRotator.h"

LogRotator::LogRotator(size_t mfs, int mb) : maxFileSize(mfs), maxBackups(mb){}

bool LogRotator::needRotation(size_t currentFileSize) const{
    if(maxFileSize == 0) return false; //if maxFileSize=0 turn off the rotator
    return currentFileSize >= maxFileSize;
}

void LogRotator::rotate(const std::string& filePath){
    if(maxBackups == 0 && std::filesystem::exists(filePath)){
        std::filesystem::remove(filePath);
        return;
}

    for (int i = maxBackups; i > 0; --i) {
        std::string oldName = filePath + "." + std::to_string(i);
        std::string newName = filePath + "." + std::to_string(i + 1);

        if (std::filesystem::exists(oldName)) {
            if (i == maxBackups) {
                std::filesystem::remove(oldName);
            } else {
                std::filesystem::rename(oldName, newName);
            }
        }
    }

    if (std::filesystem::exists(filePath)) {
        std::filesystem::rename(filePath, filePath + ".1");
    }
}