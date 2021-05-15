//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_SIMD_UNIT_H
#define RED_O_LATOR_SIMD_UNIT_H

#include <cstdint>
#include <memory>

struct ComputeUnit;

struct SimdUnit {
    ComputeUnit* cu;
    uint64_t PC;

    std::unique_ptr<uint8_t[]> vectorRegFile;

    SimdUnit(ComputeUnit* cu) :
          cu(cu),
          vectorRegFile(new uint8_t[64 * 1024]) {}
};

#endif  // RED_O_LATOR_SIMD_UNIT_H
