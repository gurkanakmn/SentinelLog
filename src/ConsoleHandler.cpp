#include "../include/ConsoleHandler.h"
#include <iostream>
#include <thread>

ConsoleHandler::ConsoleHandler(std::unique_ptr<Formatter> formatter, bool enableColor):
                                Handler(move(formatter)), enableColor(enableColor){}

void ConsoleHandler::handle(const LogRecord& record){
    std::lock_guard<std::mutex> lock(this->lock);
    const char* color = Color::RESET;
    switch(record.level){
        case LogLevel::DEBUG: color = Color::CYAN; break;
        case LogLevel::INFO: color = Color::GREEN; break;
        case LogLevel::WARNING: color = Color::YELLOW; break;
        case LogLevel::ERROR: color = Color::RED; break;
        case LogLevel::FATAL: color = Color::BOLD_RED; break;
    }

    if(enableColor){
        if(record.level >= LogLevel::ERROR){
            std::cerr << color << this->formatter->format(record) << Color::RESET << std::endl;     
        }
        else{
            std::cout << color << this->formatter->format(record) << Color::RESET << std::endl;         
        }
    }
    else{
        if(record.level >= LogLevel::ERROR){
            std::cerr << color << this->formatter->format(record) << Color::RESET << std::endl;     
        }
        else{
            std::cout << color << this->formatter->format(record) << Color::RESET << std::endl;         
        }
    }
}