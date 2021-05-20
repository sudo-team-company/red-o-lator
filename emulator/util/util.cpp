//
// Created by Diana Kudaiberdieva
//

#include "util.h"

uint16_t bit_count(uint32_t i) {
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

uint16_t bit_count(uint64_t i) {
    i = i - ((i >> 1) & 0x5555555555555555);
    i = (i & 0x3333333333333333) + ((i >> 2) & 0x3333333333333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101) >> 56;
}

uint64_t rev_bit(uint64_t n) {
    n = (n >> 32) | (n << 32);
    n = ((n & 0xffff0000ffff0000) >> 16) | ((n & 0x0000ffff0000ffff) << 16);
    n = ((n & 0xff00ff00ff00ff00) >> 8) | ((n & 0x00ff00ff00ff00ff) << 8);
    n = ((n & 0xf0f0f0f0f0f0f0f0) >> 4) | ((n & 0x0f0f0f0f0f0f0f0f) << 4);
    n = ((n & 0xCCCCCCCCCCCCCCCC) >> 2) | ((n & 0x3333333333333333) << 2);
    n = ((n & 0xAAAAAAAAAAAAAAAA) >> 1) | ((n & 0x5555555555555555) << 1);
    return n;
}

uint32_t rev_bit(uint32_t n) {
    n = (n >> 16) | (n << 16);
    n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
    n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
    n = ((n & 0XCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);
    n = ((n & 0XAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);
    return n;
}

int32_t sign_ext(int8_t i) {
    return (int32_t) i;
}

int32_t sign_ext(int16_t i) {
    return (int32_t) i;
}

uint64_t to_uin64_t(const std::vector<uint32_t>& data) {
    assert(!data.empty() && "Data can not be empty");
    uint64_t result = 0;
    auto maxInd = std::min((size_t) data.size(), size_t(2)) - 1;

    for (size_t i = 0; i <= maxInd; ++i) {
        result |= (uint64_t(data[i]) << 32 * (maxInd - i));
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
            value |= (uint32_t(data[j]) << a);
        }
        result.push_back(value);
        i += 4;
    }

    return result;
}
