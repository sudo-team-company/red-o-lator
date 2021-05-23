#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <functional>
#include <locale>
#include <optional>
#include <set>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>

namespace utils {
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

template <typename T>
static std::string joinToString(const std::vector<T>& vector,
                         const std::string& delimeter,
                         const std::string& prefix,
                         const std::string& suffix,
                         std::function<std::string(T)> transform) {
    std::string result;

    result += prefix;

    for (int i = 0; i < vector.size(); ++i) {
        result += transform(vector[i]);
        if (i != vector.size() - 1) {
            result += delimeter;
        }
    }

    result += suffix;

    return result;
}

template <typename T>
static inline std::string joinToString(const std::vector<T>& vector,
                         const std::string& delimeter,
                         std::function<std::string(T)> transform) {
    return joinToString(vector, delimeter, "", "", transform);
}

static inline std::string joinToString(const std::vector<std::string>& vector,
                         const std::string& delimeter) {
    return joinToString<std::string>(vector, delimeter, "", "", [](auto s) {
        return s;
    });
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

static inline bool startsWith(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

static inline bool endsWith(const std::string& s, const std::string& suffix) {
    return s.size() >= suffix.size() &&
           0 == s.compare(s.size() - suffix.size(), suffix.size(), suffix);
}

template <typename K, typename V>
static inline void insertOrUpdate(std::unordered_map<K, V>& map,
                                  const K& key,
                                  const V& value) {
    auto existing = map.find(key);
    if (existing != map.end()) {
        existing->second = value;
    } else {
        map.emplace(key, value);
    }
}

template <typename T>
static inline std::optional<T*> optionalOf(T* value) {
    return value ? std::make_optional(value) : std::nullopt;
}

template <typename T>
static inline std::optional<T> optionalOf(T value) {
    return std::make_optional(value);
}

template <typename T>
static inline std::optional<T> optionalOf() {
    return std::nullopt;
}

template <typename T>
static inline bool contains(std::vector<T> vector, T value) {
    return vector.find(value) != vector.end();
}

template <typename T>
static inline bool contains(std::set<T> set, T value) {
    return set.find(value) != set.end();
}

static std::vector<unsigned char> readBinaryFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input.is_open()) {
        throw std::runtime_error("Failed to open " + path);
    }

    return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),
                                      {});
}
}  // namespace utils
