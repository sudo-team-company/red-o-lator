//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

static inline void run_v_mov_b32(WfStateVOP1& state, size_t wiInd) {
    state.VDST[wiInd] = state.SRC0[wiInd];
}

void run_vop1(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vop1_state(instr);
    for (size_t wiInd = 0; wiInd < wf->workItems.size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instr.get_key()) {
            case V_MOV_B32:
                run_v_mov_b32(state, wiInd);
                break;
        }
    }
    wf->update_with_vop1_state(instr, state);
}
