//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_SCALAR_UNIT_H
#define RED_O_LATOR_SCALAR_UNIT_H

#include <memory>

struct ComputeUnit;

struct ScalarUnit {
    ComputeUnit* cu;

    std::unique_ptr<uint8_t[]> scalarRegFile;

    ScalarUnit(ComputeUnit* cu):
          cu(cu),
          scalarRegFile(new uint8_t[8 * 1024]) {}

};

#endif  // RED_O_LATOR_SCALAR_UNIT_H
