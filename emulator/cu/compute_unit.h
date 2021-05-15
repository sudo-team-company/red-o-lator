//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_COMPUTE_UNIT_H
#define RED_O_LATOR_COMPUTE_UNIT_H

#include <array>
#include "scalar_unit.h"
#include "scheduler.h"
#include "simd_unit.h"

#define SIMD_COUNT 4

struct ComputeUnit {
    std::array<std::unique_ptr<SimdUnit>, SIMD_COUNT> simds;
    std::unique_ptr<ScalarUnit> scalarUnit;
    std::unique_ptr<uint8_t[]> localDS;
    std::unique_ptr<Scheduler> scheduler;

    ComputeUnit()
        : scalarUnit(new ScalarUnit(this)),
          localDS(new uint8_t[64 * 1024]),
          scheduler(new Scheduler(this)) {
        std::fill(simds.begin(), simds.end(), new SimdUnit(this));
    }

    static ComputeUnit build();
};



#endif  // RED_O_LATOR_COMPUTE_UNIT_H
