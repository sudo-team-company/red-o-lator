#pragma once

#include <unordered_map>

namespace utils {
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
}  // namespace utils
