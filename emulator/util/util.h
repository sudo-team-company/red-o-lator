//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_UTIL_H
#define RED_O_LATOR_UTIL_H

#include <type_traits>
#include <cassert>
#include <cstdint>

uint16_t bit_count(uint32_t);

uint16_t bit_count(uint64_t);

uint32_t rev_bit(uint32_t);

uint64_t rev_bit(uint64_t);

int32_t sign_ext(int8_t);

int32_t sign_ext(int16_t);

template<typename T>
static inline
constexpr bool get_bit(uint8_t n, T value) {
    static_assert(std::is_trivial<T>::value, "Trivial type expected");
    assert((n < sizeof(value) * 8) && "Bit index out of bound");

    return (value & (static_cast<T>(1) << n)) != 0;
}

#endif  // RED_O_LATOR_UTIL_H
