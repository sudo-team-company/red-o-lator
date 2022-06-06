#ifndef RED_O_LATOR_BIT_UTILS_HPP
#define RED_O_LATOR_BIT_UTILS_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>

template <typename T>
static inline constexpr bool get_bit(uint8_t n, T value) {
    static_assert(std::is_trivial<T>::value, "Trivial type expected");
    assert((n < sizeof(value) * 8) && "Bit index out of bound");

    return (value & (static_cast<T>(1) << n)) != 0;
}


#endif  // RED_O_LATOR_BIT_UTILS_HPP
