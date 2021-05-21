//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_COMPUTE_UNIT_H
#define RED_O_LATOR_COMPUTE_UNIT_H

#include <array>
#include "../alu/alu.h"
#include "../flow/wavefront.h"

struct ComputeUnit {

    void run_work_group(WorkGroup* wg);
    void run_wavefront(Wavefront*);

   private:
    void resolve_barrier(WorkGroup* wg);
};

#endif  // RED_O_LATOR_COMPUTE_UNIT_H
