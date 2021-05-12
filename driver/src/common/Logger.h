#pragma once

#include <ostream>
#include <string>

class Logger {
   public:
    void log(const std::string& message);
    void log(const char* message);

    void error(const std::string& message);
    void error(const char* message);

   private:
    std::string mPrefix = "[red-o-lator] --- ";

    std::string appendPrefix(const char* message);
};
