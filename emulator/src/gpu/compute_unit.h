#pragma once

#include <array>
#include "alu/alu.h"
#include "flow/wavefront.h"

struct ComputeUnit {
    static void run_work_group(WorkGroup* wg);
    static void run_wavefront(Wavefront*);

   private:
    static void resolve_barrier(WorkGroup* wg);
};
