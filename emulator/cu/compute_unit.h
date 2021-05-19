//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_COMPUTE_UNIT_H
#define RED_O_LATOR_COMPUTE_UNIT_H

#include <array>
#include "../flow/wavefront.h"
#include "../alu/alu.h"

struct ComputeUnit {
//    std::unique_ptr<uint8_t[]> localDS;
//    std::unique_ptr<Scheduler> scheduler;

    ComputeUnit() {}
//          localDS(new uint8_t[64 * 1024]),
//          scheduler(new Scheduler(this))

    void run_work_group(WorkGroup* wg);

    void run_wavefront(Wavefront*);

   private:
    void resolve_barrier(WorkGroup* wg);
};



#endif  // RED_O_LATOR_COMPUTE_UNIT_H
