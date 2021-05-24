#pragma once

#include <set>

namespace utils {
template <typename T>
static inline bool contains(std::set<T> set, T value) {
    return set.find(value) != set.end();
}
}
