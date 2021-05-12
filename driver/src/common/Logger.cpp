#include <iostream>
#include <string>

#include "Logger.h"

void Logger::log(const char* message) {
    std::cout << appendPrefix(message) << std::endl;
}

void Logger::error(const char* message) {
    std::cerr << appendPrefix(message) << std::endl;
}

void Logger::log(const std::string& message) {
    log(message.c_str());
}

void Logger::error(const std::string& message) {
    error(message.c_str());
}

std::string Logger::appendPrefix(const char* message) {
    return mPrefix + message;
}
