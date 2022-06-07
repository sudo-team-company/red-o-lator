#pragma once

#include <array>
#include "alu/alu.h"
#include "flow/debug_context.h"
#include "flow/wavefront.h"

struct ComputeUnit {
    static void run_work_group(WorkGroup* wg, DebugContext& debug);
    static void run_wavefront(Wavefront*, DebugContext& debug);

   private:
    static void resolve_barrier(WorkGroup* wg);
};
