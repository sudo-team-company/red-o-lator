#pragma once

#include <algorithm>
#include <cctype>
#include <locale>
#include <vector>

static std::vector<std::string> split(const std::string& line,
                                      const char separator) {
    if (line.empty()) {
        return {""};
    }

    std::vector<std::string> result;
    std::string buffer;

    for (char ch : line) {
        if (ch != separator) {
            buffer += ch;
        } else {
            result.push_back(buffer);
            buffer = "";
        }
    }

    if (!buffer.empty()) {
        result.push_back(buffer);
    }

    return result;
}

static std::pair<std::string, std::string> splitTwo(const std::string& line,
                                                    const char separator) {
    const auto splitLine = split(line, separator);

    if (splitLine.size() < 2) {
        throw std::logic_error("Could not split line into two parts!");
    }

    return std::make_pair(splitLine[0], splitLine[1]);
}

static inline void ltrimInplace(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

static inline void rtrimInplace(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) {
                             return !std::isspace(ch);
                         })
                .base(),
            s.end());
}

static inline void trimInplace(std::string& s) {
    ltrimInplace(s);
    rtrimInplace(s);
}

static inline std::string ltrim(std::string s) {
    ltrimInplace(s);
    return s;
}

static inline std::string rtrim(std::string s) {
    rtrimInplace(s);
    return s;
}

static inline std::string trim(std::string s) {
    trimInplace(s);
    return s;
}

