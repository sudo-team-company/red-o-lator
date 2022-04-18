#include "util.h"

uint32_t to_uint32_t(float f) {
    uint32_t value = 0;
    memcpy(&value, &f, sizeof value);
    return value;
}

uint64_t to_uin64_t(const std::vector<uint32_t>& data) {
    assert(!data.empty() && "Data can not be empty");
    uint64_t result = 0;
    auto maxInd = std::min(static_cast<size_t>(data.size()), size_t(2)) - 1;

    for (size_t i = 0; i <= maxInd; ++i) {
        result |= (static_cast<uint64_t>(data[i]) << 32 * (maxInd - i));
    }

    return result;
}

std::vector<uint32_t> to_uint32_v(const std::vector<uint8_t>& data) {
    auto result = std::vector<uint32_t>();

    // (byte7 | byte6 | byte5 | byte4) (byte3 | byte2 | byte1 | byte0)
    // (uint32_t 1) | (uint32_t 0)
    // (uint64_t 0)
    assert(data.size() % 4 == 0 && "Data was not aligned for uint32_t conversion");

    size_t i = 0;
    while (i < data.size()) {
        uint32_t value = 0;
        for (size_t j = i; j < i + 4; ++j) {
            auto a = 8 * (i + 3 - j);
            value |= (static_cast<uint32_t>(data[j]) << a);
        }
        result.push_back(value);
        i += 4;
    }

    return result;
}

bool is_hex(const std::string &str) {
    if (utils::startsWith(str, "0x")) {
        return str.size() > 2 && std::all_of(str.begin() + 2, str.end(), ::isxdigit);
    }
    return std::all_of(str.begin(), str.end(), ::isxdigit);
}

bool is_integer(const std::string &str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}

