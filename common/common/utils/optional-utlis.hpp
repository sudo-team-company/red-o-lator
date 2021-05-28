#pragma once

#include <optional>

namespace utils {
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
}
