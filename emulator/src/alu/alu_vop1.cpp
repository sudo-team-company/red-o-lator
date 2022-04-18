//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

static inline void run_v_bfrev_b32(WfStateVOP1& state, size_t wiInd) {
    state.VDST[wiInd] = rev_bit(static_cast<uint32_t>(state.SRC0[wiInd]));
}

static inline void run_v_ffbh_u32(WfStateVOP1& state, size_t wiInd) {
    int32_t VDST = -1;
    for (uint8_t i = 31; i >= 0; i--)
        if (((uint64_t(1) << i) & state.SRC0[wiInd]) != 0) {
            VDST = 31 - i;
            break;
        }
    state.VDST[wiInd] = VDST;
}

static inline void run_v_ffbh_i32(WfStateVOP1& state, size_t wiInd) {
     int32_t VDST = -1;
     uint32_t bitval = static_cast<int32_t>(state.SRC0[wiInd])>=0 ? 1 : 0;
     for (uint8_t i = 31; i >= 0; i--)
        if (((uint64_t(1)<<i) & state.SRC0[wiInd]) == (bitval<<i)) {
        VDST = 31 - i;
        break;
    }
    state.VDST[wiInd] = VDST;
}

static inline void run_v_ffbl_b32(WfStateVOP1& state, size_t wiInd) {
    int32_t VDST = -1;
    for(uint8_t i = 0; i < 32; ++i) {
        if (((uint64_t(1) << i) & state.SRC0[wiInd]) != 0) {
            VDST = i;
            break;
        }
    }
    state.VDST[wiInd] = VDST;
}

static inline void run_v_mov_b32(WfStateVOP1& state, size_t wiInd) {
    state.VDST[wiInd] = state.SRC0[wiInd];
}

void run_vop1(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vop1_state(instr);
    for (size_t wiInd = 0; wiInd < wf->workItems.size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instr.get_key()) {
            case V_BFREV_B32:
                run_v_bfrev_b32(state, wiInd);
                break;
            case V_FFBH_U32:
                run_v_ffbh_u32(state, wiInd);
                break;
            case V_FFBH_I32:
                run_v_ffbh_i32(state, wiInd);
                break;
            case V_FFBL_B32:
                run_v_ffbl_b32(state, wiInd);
                break;
            case V_MOV_B32:
                run_v_mov_b32(state, wiInd);
                break;
          default:
              unsupported_instruction("VOP1", get_instr_str(instr.get_key()));
        }
    }
    wf->update_with_vop1_state(instr, state);
}
