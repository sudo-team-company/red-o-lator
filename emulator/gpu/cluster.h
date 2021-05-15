//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_CLUSTER_H
#define RED_O_LATOR_CLUSTER_H

#define CLUSTER_CU_AMOUTN 4

#include "../cu/compute_unit.h"

struct Cluster {
    std::array<std::unique_ptr<ComputeUnit>, CLUSTER_CU_AMOUTN> computeUnits;

    Cluster() {
        std::fill(computeUnits.begin(), computeUnits.end(), new ComputeUnit());
    }
};

#endif  // RED_O_LATOR_CLUSTER_H
