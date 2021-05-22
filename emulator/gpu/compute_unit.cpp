//
// Created by Diana Kudaiberdieva
//

#include "compute_unit.h"

void ComputeUnit::run_work_group(WorkGroup* wg) {
    while (!wg->all_wf_completed()) {
        for (auto& wavefront : wg->wavefronts) {
            auto* wf = wavefront.get();
            run_wavefront(wf);
        }
        resolve_barrier(wg);
    }
}

void ComputeUnit::run_wavefront(Wavefront* wavefront) {
    while (true) {
        Instruction* curInstr = wavefront->get_cur_instr();
        InstrKey instrKey = curInstr->get_key();

        if (instrKey == S_ENDPGM) {
            wavefront->completed = true;
            break;
        }

        if (instrKey == S_BARRIER) {
            wavefront->atBarrier = true;
            break;
        }
        if (instrKey != S_WAITCNT) {  // S_WAITCNT is always ignored
            run_instr(*curInstr, wavefront);
        }
        wavefront->to_next_instr();
    }
}

void ComputeUnit::resolve_barrier(WorkGroup* wg) {
    if (wg->all_wf_completed()) return;
    for (auto& wavefront : wg->wavefronts) {
        auto* wf = wavefront.get();
        if (!wf->atBarrier) assert(false && "Illegal wavefront state");
        wf->atBarrier = false;
    }
}
