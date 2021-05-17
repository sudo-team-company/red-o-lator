//
// Created by Diana Kudaiberdieva
//

#include <stdexcept>
#include "alu.h"

void run_s_bitcmp0_b32(WfStateSOPC& state) {
    state.SCC = (state.SSRC0 & (uint32_t(1) << (state.SSRC1 & 31))) == 0;
}

void run_s_bitcmp0_b64(WfStateSOPC& state) {
    state.SCC = (state.SSRC0 & (uint64_t(1) << (state.SSRC1 & 63))) == 0;
}

void run_s_bitcmp1_b32(WfStateSOPC& state) {
    state.SCC = (state.SSRC0 & (uint32_t(1) << (state.SSRC1 & 31))) != 0;
}

void run_s_bitcmp1_b64(WfStateSOPC& state) {
    state.SCC = (state.SSRC0 & (uint64_t(1) << (state.SSRC1 & 63))) != 0;
}

void run_s_cmp_eq_u64(WfStateSOPC& state) {
    state.SCC = state.SSRC0 == state.SSRC1;
}

void run_s_cmp_eq_i32(WfStateSOPC& state) {
    run_s_cmp_eq_u64(state);
}

void run_s_cmp_eq_u32(WfStateSOPC& state) {
    run_s_cmp_eq_u64(state);
}

void run_s_cmp_ge_i32(WfStateSOPC& state) {
    state.SCC = int32_t(state.SSRC0) >= int32_t(state.SSRC1);
}

void run_s_cmp_ge_u32(WfStateSOPC& state) {
    state.SCC = uint32_t(state.SSRC0) >= uint32_t(state.SSRC1);
}

void run_s_cmp_gt_i32(WfStateSOPC& state) {
    state.SCC = int32_t(state.SSRC0) > int32_t(state.SSRC1);
}

void run_s_cmp_gt_u32(WfStateSOPC& state) {
    state.SCC = uint32_t(state.SSRC0) > uint32_t(state.SSRC1);
}

void run_s_cmp_le_i32(WfStateSOPC& state) {
    state.SCC = int32_t(state.SSRC0) <= int32_t(state.SSRC1);
}

void run_s_cmp_le_u32(WfStateSOPC& state) {
    state.SCC = uint32_t(state.SSRC0) <= uint32_t(state.SSRC1);
}

void run_s_cmp_lg_u64(WfStateSOPC& state) {
    state.SCC = state.SSRC0 != state.SSRC1;
}

void run_s_cmp_lg_i32(WfStateSOPC& state) {
    run_s_cmp_lg_u64(state);
}
void run_s_cmp_lg_u32(WfStateSOPC& state) {
    run_s_cmp_lg_u64(state);
}

void run_s_cmp_ne_u64(WfStateSOPC& state) {
    run_s_cmp_lg_u64(state);
}

void run_s_cmp_lt_i32(WfStateSOPC& state) {
    state.SCC = int32_t(state.SSRC0) < int32_t(state.SSRC1);
}
void run_s_cmp_lt_u32(WfStateSOPC& state) {
    state.SCC = state.SSRC0 < state.SSRC1;
}
void run_s_set_gpr_idx_on(WfStateSOPC& state) {
    state.MODE.gpr_idx_en(1);
    // todo differs from official doc
    state.M0 = ((state.IMM8 & 15) << 12) | (state.SSRC0 & 0xff);
}
void run_s_setvskip(WfStateSOPC& state) {
    state.MODE.vskip((state.SSRC0 & 1 << (state.SSRC1 & 31)) != 0);
}


void run_sopc(InstrKey instr, WfStateSOPC& state) {
    switch (instr) {
        case S_BITCMP0_B32:
            run_s_bitcmp0_b32(state);
            break;
        case S_BITCMP0_B64:
            run_s_bitcmp0_b64(state);
            break;
        case S_BITCMP1_B32:
            run_s_bitcmp1_b32(state);
            break;
        case S_BITCMP1_B64:
            run_s_bitcmp1_b64(state);
            break;
        case S_CMP_EQ_I32:
            run_s_cmp_eq_i32(state);
            break;
        case S_CMP_EQ_U32:
            run_s_cmp_eq_u32(state);
            break;
        case S_CMP_EQ_U64:
            run_s_cmp_eq_u64(state);
            break;
        case S_CMP_GE_I32:
            run_s_cmp_ge_i32(state);
            break;
        case S_CMP_GE_U32:
            run_s_cmp_ge_u32(state);
            break;
        case S_CMP_GT_I32:
            run_s_cmp_gt_i32(state);
            break;
        case S_CMP_GT_U32:
            run_s_cmp_gt_u32(state);
            break;
        case S_CMP_LE_I32:
            run_s_cmp_le_i32(state);
            break;
        case S_CMP_LE_U32:
            run_s_cmp_le_u32(state);
            break;
        case S_CMP_LG_I32:
            run_s_cmp_lg_i32(state);
            break;
        case S_CMP_LG_U32:
            run_s_cmp_lg_u32(state);
            break;
        case S_CMP_LG_U64:
            run_s_cmp_lg_u64(state);
            break;
        case S_CMP_NE_U64:
            run_s_cmp_ne_u64(state);
            break;
        case S_CMP_LT_I32:
            run_s_cmp_lt_i32(state);
            break;
        case S_CMP_LT_U32:
            run_s_cmp_lt_u32(state);
            break;
        case S_SET_GPR_IDX_ON:
            run_s_set_gpr_idx_on(state);
            break;
        case S_SETVSKIP:
            run_s_setvskip(state);
            break;
        default:
            assert(false && "Unknown instruction met!");
            throw std::runtime_error(std::string("Unexpected instruction key: ") + get_instr_str(instr));
    }
}