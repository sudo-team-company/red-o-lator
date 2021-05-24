#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <functional>
#include <locale>
#include <string_view>
#include <vector>

namespace utils {
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

static inline bool startsWith(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

static inline bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix);
}

static std::vector<std::string> split(const std::string& line,
                                      const char separator,
                                      int maxSplitCount = -1) {
    if (line.empty()) {
        return {""};
    }

    std::vector<std::string> result;
    std::string buffer;

    for (char ch : line) {
        if (ch != separator) {
            buffer += ch;
        } else if (maxSplitCount < 0 || result.size() < maxSplitCount) {
            result.push_back(buffer);
            buffer = "";
        } else {
            buffer += ch;
        }
    }

    if (!buffer.empty()) {
        result.push_back(buffer);
    }

    return result;
}

template <typename R>
static std::vector<R> splitMap(
    const std::string& line,
    const char separator,
    int maxSplitCount = -1,
    std::function<R(std::string&)> transform = nullptr) {
    assert(transform);
    auto splitResult = split(line, separator, maxSplitCount);
    std::vector<std::string> result;
    std::transform(splitResult.begin(), splitResult.end(),
                   std::back_inserter(result), transform);
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
}
