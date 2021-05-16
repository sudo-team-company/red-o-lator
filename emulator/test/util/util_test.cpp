//
// Created by Diana Kudaiberdieva
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <common/test/doctest.h>

#include "../../util/util.h"

TEST_CASE("bit_counts - count bits") {
    CHECK(bit_count(uint32_t(0xffffffff)) == 32);
    CHECK(bit_count(uint32_t(0x01234567)) == 12);
    CHECK(bit_count(uint32_t(0x0)) == 0);
    CHECK(bit_count(uint64_t(0xffffffffffffffff)) == 64);
    CHECK(bit_count(uint64_t(0x0123456789abcdef)) == 32);
    CHECK(bit_count(uint64_t(0x0)) == 0);
}

TEST_CASE("sign_ext - sign extension") {
    uint64_t value = -128;
    SUBCASE("sign extension for negative value") {
        auto value_int8 = (int8_t)value;
        CHECK(sign_ext(value_int8) == -128);
        auto value_int16 = (int16_t)value;
        CHECK(sign_ext(value_int16) == -128);
    }
}

TEST_CASE("rev_bit - reverses bits") {
    uint64_t value64 = 0x1234567890ABCDEF; //0001001000110100010101100111100010010000101010111100110111101111
    SUBCASE("reverses bits of uint64 value") {
        uint64_t expected = 0xF7B3D5091E6A2C48; //1111011110110011110101010000100100011110011010100010110001001000
        CHECK(rev_bit(value64) == expected);
    }
    uint32_t value32 = 0x0F53DE27; //00001111010100111101111000100111
    SUBCASE("reverses bits of uint64 value") {
        uint32_t expected = 0xE47BCAF0; //11100100011110111100101011110000
        CHECK(rev_bit(value32) == expected);
    }
}