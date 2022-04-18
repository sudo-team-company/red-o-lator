#pragma once

#include <type_traits>
#include <cassert>
#include <cstdint>
#include <vector>
#include <common/utils/common.hpp>

constexpr uint16_t bit_count(uint32_t n) {
    n = n - ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    return (((n + (n >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

constexpr uint16_t bit_count(uint64_t n) {
    n = n - ((n >> 1) & 0x5555555555555555);
    n = (n & 0x3333333333333333) + ((n >> 2) & 0x3333333333333333);
    return (((n + (n >> 4)) & 0x0F0F0F0F0F0F0F0F) * 0x0101010101010101) >> 56;
}

constexpr uint32_t rev_bit(uint32_t n){
    n = (n >> 16) | (n << 16);
    n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
    n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
    n = ((n & 0XCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);
    n = ((n & 0XAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);
    return n;
}

constexpr uint64_t rev_bit(uint64_t n){
    n = (n >> 32) | (n << 32);
    n = ((n & 0xffff0000ffff0000) >> 16) | ((n & 0x0000ffff0000ffff) << 16);
    n = ((n & 0xff00ff00ff00ff00) >> 8) | ((n & 0x00ff00ff00ff00ff) << 8);
    n = ((n & 0xf0f0f0f0f0f0f0f0) >> 4) | ((n & 0x0f0f0f0f0f0f0f0f) << 4);
    n = ((n & 0xCCCCCCCCCCCCCCCC) >> 2) | ((n & 0x3333333333333333) << 2);
    n = ((n & 0xAAAAAAAAAAAAAAAA) >> 1) | ((n & 0x5555555555555555) << 1);
    return n;
}

constexpr int32_t sign_ext(int8_t i) {
    return static_cast<int32_t>(i);
}

constexpr int32_t sign_ext(int16_t i) {
    return static_cast<int32_t>(i);
}

bool is_hex(const std::string &);

bool is_integer(const std::string &);

uint32_t to_uint32_t(float);

uint64_t to_uin64_t(const std::vector<uint32_t>&);

std::vector<uint32_t> to_uint32_v(const std::vector<uint8_t>&);

template<typename T>
static inline
constexpr bool get_bit(uint8_t n, T value) {
    static_assert(std::is_trivial<T>::value, "Trivial type expected");
    assert((n < sizeof(value) * 8) && "Bit index out of bound");

    return (value & (static_cast<T>(1) << n)) != 0;
}

