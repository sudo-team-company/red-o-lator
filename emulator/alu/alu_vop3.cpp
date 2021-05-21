//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

static inline void run_v_mul_lo_u32(WfStateVOP3& state) {
    state.VDST = state.SRC0 * state.SRC1;
}

void run_vop3(const Instruction& instr, Wavefront* wf) {
//    switch (instr.get_key()) {
//        case V_MUL_LO_U32:
//            run_v_mul_lo_u32(state);
//            break;
//    }
}
