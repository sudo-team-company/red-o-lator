//
// Created by Diana Kudaiberdieva on 12.05.2021.
//

#ifndef RED_O_LATOR_COMPUTE_UNIT_H
#define RED_O_LATOR_COMPUTE_UNIT_H

#include <array>
#include <memory>
#include "simd_unit.h"
#include "scalar_unit.h"

struct ComputeUnit {
    std::unique_ptr<ScalarUnit> scalarUnit;

    std::array<std::unique_ptr<SimdUnit>, 4> simds;

};

#endif  // RED_O_LATOR_COMPUTE_UNIT_H
