//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

static inline void run_v_mul_lo_u32(WfStateVOP3& state, size_t i) {
    state.VDST[i] = state.SRC0[i] * state.SRC1[i];
}

void run_vop3(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vop3_state(instr);
    for (size_t wiInd = 0; wiInd < wf->workItems.size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instr.get_key()) {
            case V_MUL_LO_U32:
                run_v_mul_lo_u32(state, wiInd);
                break;
        }
    }
    wf->update_with_vop3_state(instr, state);
}
