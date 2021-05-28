#pragma once

#include <algorithm>
#include <functional>
#include <locale>
#include <vector>
#include <type_traits>

namespace utils {
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
static inline std::string joinToString(
    const std::vector<T>& vector,
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

template <
    typename T,  // real type
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
static inline std::string joinToString(const std::vector<T>& vector,
                                       const std::string& delimeter) {
    return joinToString<T>(vector, delimeter, "", "", [](auto s) {
      return std::to_string(s);
    });
}

template <typename T>
static inline bool contains(std::vector<T> vector, T value) {
    return vector.find(value) != vector.end();
}
}
