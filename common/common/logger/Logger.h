#pragma once

#include <ostream>
#include <string>
#include <utility>

#define LOG_LEVEL_NONE 0
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_TEMP 4
#define LOG_LEVEL_DEFAULT LOG_LEVEL_WARN

#if defined(RED_O_LATOR_LOG_LEVEL_ERROR)
#define LOG_LEVEL LOG_LEVEL_ERROR
#elif defined(RED_O_LATOR_LOG_LEVEL_WARN)
#define LOG_LEVEL LOG_LEVEL_WARN
#elif defined(RED_O_LATOR_LOG_LEVEL_DEBUG)
#define LOG_LEVEL LOG_LEVEL_DEBUG
#elif defined(RED_O_LATOR_LOG_LEVEL_TEMP)
#define LOG_LEVEL LOG_LEVEL_TEMP
#elif defined(RED_O_LATOR_LOG_LEVEL_NONE)
#define LOG_LEVEL LOG_LEVEL_NONE
#else
#define LOG_LEVEL LOG_LEVEL_DEFAULT
#endif

class Logger {
   public:
    Logger() = default;
    explicit Logger(std::string mPrefix) : mPrefix(std::move(mPrefix)) {}

    void temp(const std::string& message);
    void temp(const char* message);

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
