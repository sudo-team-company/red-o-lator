#pragma once

#include <ostream>
#include <string>
#include <utility>

class Logger {
   public:
    Logger() = default;
    explicit Logger(std::string mPrefix) : mPrefix(std::move(mPrefix)) {}

    void debug(const std::string& message);
    void debug(const char* message);

    void warn(const std::string& message);
    void warn(const char* message);

    void error(const std::string& message);
    void error(const char* message);

   private:
    std::string mPrefix = "[red-o-lator]";

    std::string appendPrefix(const char* message, const char* level);
};
