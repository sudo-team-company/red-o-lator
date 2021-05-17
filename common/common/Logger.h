#pragma once

#include <ostream>
#include <string>
#include <utility>

class Logger {
   public:
    Logger() = default;
    explicit Logger(std::string mPrefix) : mPrefix(std::move(mPrefix)) {}

    void log(const std::string& message);
    void log(const char* message);

    void error(const std::string& message);
    void error(const char* message);

   private:
    std::string mPrefix = "[red-o-lator] ---";

    std::string appendPrefix(const char* message);
};
