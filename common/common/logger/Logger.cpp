#include <iostream>
#include <string>

#include "Logger.h"

void Logger::debug(const char* message) {
    std::cout << appendPrefix(message, "DEBUG") << std::endl;
}

void Logger::warn(const char* message) {
    std::cout << appendPrefix(message, "WARN") << std::endl;
}

void Logger::error(const char* message) {
    std::cerr << appendPrefix(message, "ERROR") << std::endl;
}

void Logger::debug(const std::string& message) {
    debug(message.c_str());
}

void Logger::warn(const std::string& message) {
    warn(message.c_str());
}

void Logger::error(const std::string& message) {
    error(message.c_str());
}

std::string Logger::appendPrefix(const char* message, const char* level) {
    return mPrefix + " --- " + level + " --- " + message;
}
