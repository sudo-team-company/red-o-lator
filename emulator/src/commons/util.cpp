#include "util.h"
#include "commons/commons.h"

uint32_t to_uint32_t(float f) {
    uint32_t value = 0;
    std::memcpy(&value, &f, sizeof value);
    return value;
}

uint64_t to_uin64_t(const std::vector<uint32_t> &data) {
    assert(!data.empty() && "Data can not be empty");
    uint64_t result = 0;
    auto maxInd = std::min(static_cast<size_t>(data.size()), size_t(2)) - 1;

    for (size_t i = 0; i <= maxInd; ++i) {
        result |= (static_cast<uint64_t>(data[i]) << 32 * (maxInd - i));
    }

    return result;
}
uint64_t to_uin64_t(uint32_t msb, uint32_t lsb) {
    return to_uin64_t({msb, lsb});
}

/**
 *  result[n-1] contains LSB
 *  result[0] contains MSB
 */
std::vector<uint32_t> to_uint32_v(const std::vector<uint8_t> &data) {
    auto result = std::vector<uint32_t>();
    assert(data.size() % 4 == 0 && "Data was not aligned for uint32_t conversion");
    for (size_t i = 0; i < data.size(); i += 4) {
        uint32_t value = 0;
        for (size_t j = i; j < i + 4; ++j) {
            auto a = 8 * (i + 3 - j);
            value |= (static_cast<uint32_t>(data[j]) << a);
        }
        result.push_back(value);
    }
    return result;
}

float to_float(uint32_t n) {
    float result = 0.0f;
    std::memcpy(&result, &n, sizeof(n));
    return result;
}
float to_float(uint64_t n) {
    if (n >> 32) {
        logger.error(std::string("Lose information during conversion u64 to f32: ") + (std::bitset<64>(n)).to_string());
    }
    return to_float(uint32_t(n));
}
uint64_t to_uint64(float f) {
    uint64_t result = 0ull;
    std::memcpy(&result, &f, sizeof(f));
    return result;
}
