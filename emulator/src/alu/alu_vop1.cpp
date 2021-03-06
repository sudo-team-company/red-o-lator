#include "alu.h"

static inline void run_v_not_b32(WfStateVOP1& state, size_t wiInd) {
    state.VDST[wiInd] = ~state.SRC0[wiInd];
}

static inline void run_v_sat_pk_u8_i16(WfStateVOP1& state, size_t wiInd) {
    state.VDST[wiInd] = std::max(std::min((int16_t) (state.SRC0[wiInd] & 0xffff), int16_t(255)), int16_t(0));
    state.VDST[wiInd] |= std::max(std::min((int16_t)(state.SRC0[wiInd] >> 16), int16_t(255)), int16_t(0)) << 8;
}
static inline void run_v_screen_partition_4se_b32(WfStateVOP1& state, size_t wiInd) {
    uint8_t table[256] = {
        0x1, 0x3, 0x7, 0xf, 0x5, 0xf, 0xf, 0xf, 0x7, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,
        0xf, 0x2, 0x6, 0xe, 0xf, 0xa, 0xf, 0xf, 0xf, 0xb, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf,
        0xd, 0xf, 0x4, 0xc, 0xf, 0xf, 0x5, 0xf, 0xf, 0xf, 0xd, 0xf, 0xf, 0xf, 0xf, 0xf,
        0x9, 0xb, 0xf, 0x8, 0xf, 0xf, 0xf, 0xa, 0xf, 0xf, 0xf, 0xe, 0xf, 0xf, 0xf, 0xf,
        0xf, 0xf, 0xf, 0xf, 0x4, 0xc, 0xd, 0xf, 0x6, 0xf, 0xf, 0xf, 0xe, 0xf, 0xf, 0xf,
        0xf, 0xf, 0xf, 0xf, 0xf, 0x8, 0x9, 0xb, 0xf, 0x9, 0x9, 0xf, 0xf, 0xd, 0xf, 0xf,
        0xf, 0xf, 0xf, 0xf, 0x7, 0xf, 0x1, 0x3, 0xf, 0xf, 0x9, 0xf, 0xf, 0xf, 0xb, 0xf,
        0xf, 0xf, 0xf, 0xf, 0x6, 0xe, 0xf, 0x2, 0x6, 0xf, 0xf, 0x6, 0xf, 0xf, 0xf, 0x7,
        0xb, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x2, 0x3, 0xb, 0xf, 0xa, 0xf, 0xf, 0xf,
        0xf, 0x7, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x1, 0x9, 0xd, 0xf, 0x5, 0xf, 0xf,
        0xf, 0xf, 0xe, 0xf, 0xf, 0xf, 0xf, 0xf, 0xe, 0xf, 0x8, 0xc, 0xf, 0xf, 0xa, 0xf,
        0xf, 0xf, 0xf, 0xd, 0xf, 0xf, 0xf, 0xf, 0x6, 0x7, 0xf, 0x4, 0xf, 0xf, 0xf, 0x5,
        0x9, 0xf, 0xf, 0xf, 0xd, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x8, 0xc, 0xe, 0xf,
        0xf, 0x6, 0x6, 0xf, 0xf, 0xe, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0xf, 0x4, 0x6, 0x7,
        0xf, 0xf, 0x6, 0xf, 0xf, 0xf, 0x7, 0xf, 0xf, 0xf, 0xf, 0xf, 0xb, 0xf, 0x2, 0x3,
        0x9, 0xf, 0xf, 0x9, 0xf, 0xf, 0xf, 0xb, 0xf, 0xf, 0xf, 0xf, 0x9, 0xd, 0xf, 0x1};
    state.VDST[wiInd] = table[state.SRC0[wiInd] & 0xff];
}
static inline void run_v_swap_b32(WfStateVOP1& state, size_t wiInd) {
    uint32_t tmp = state.VDST[wiInd];
    state.VDST[wiInd] = state.SRC0[wiInd];
    state.SRC0[wiInd] = tmp;
}

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
    for (size_t wiInd = 0; wiInd < wf->get_size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instr.get_key()) {
            case V_SWAP_B32:
                run_v_swap_b32(state, wiInd);
                break;
            case V_NOT_B32:
                run_v_not_b32(state, wiInd);
                break;
            case V_SAT_PK_U8_I16:
                run_v_sat_pk_u8_i16(state, wiInd);
                break;
            case V_SCREEN_PARTITION_4SE_B32:
                run_v_screen_partition_4se_b32(state, wiInd);
                break;
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
              UNSUPPORTED_INSTRUCTION("VOP1", get_mnemonic(instr.get_key()));
        }
    }
    wf->update_with_vop1_state(instr, state);
}
