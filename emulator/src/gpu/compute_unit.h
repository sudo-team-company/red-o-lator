//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_COMPUTE_UNIT_H
#define RED_O_LATOR_COMPUTE_UNIT_H

#include <array>
#include "src/alu/alu.h"
#include "src/flow/wavefront.h"

struct ComputeUnit {
    static void run_work_group(WorkGroup* wg);
    static void run_wavefront(Wavefront*);

   private:
    static void resolve_barrier(WorkGroup* wg);
};

#endif  // RED_O_LATOR_COMPUTE_UNIT_H
