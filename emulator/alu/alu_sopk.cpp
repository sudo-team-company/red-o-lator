//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

void run_s_addk_i32(WfStateSOPK& state) {
    state.SDST = int32_t(state.SDST) + int32_t(state.IMM16);
    int64_t temp = int64_t(state.SDST) + int64_t(state.IMM16);
    state.SCC = get_bit(31, temp) == get_bit(15, state.IMM16) &&
                get_bit(31, temp) != get_bit(31, int32_t(state.SDST));
}
void run_s_call_b64(WfStateSOPK& state) {
    state.SDST = state.PC + 4;
    state.PC = state.RELADDR;
}
void run_s_cbranch_i_fork(WfStateSOPK& state) {
    // todo
}
void run_s_cmovk_i32(WfStateSOPK& state) {
    if (state.SCC) state.SDST = int32_t(state.IMM16);
}
void run_s_cmpk_eq_i32(WfStateSOPK& state) {
    state.SCC = int32_t(state.SDST) == int32_t(state.IMM16);
}
void run_s_cmpk_eq_u32(WfStateSOPK& state) {
    state.SCC = state.SDST == state.IMM16;
}
void run_s_cmpk_ge_i32(WfStateSOPK& state) {
    state.SCC = int32_t(state.SDST) >= int32_t(state.IMM16);
}
void run_s_cmpk_ge_u32(WfStateSOPK& state) {
    state.SCC = state.SDST >= state.IMM16;
}
void run_s_cmpk_gt_i32(WfStateSOPK& state) {
    state.SCC = int32_t (state.SDST) >= int32_t(state.IMM16);
}
void run_s_cmpk_gt_u32(WfStateSOPK& state) {
    state.SCC = state.SDST > state.IMM16;
}
void run_s_cmpk_le_i32(WfStateSOPK& state) {
    state.SCC = int32_t (state.SDST) <= int32_t (state.IMM16);
}
void run_s_cmpk_le_u32(WfStateSOPK& state) {
    state.SCC = state.SDST <= state.IMM16;
}
void run_s_cmpk_lg_i32(WfStateSOPK& state) {
    state.SCC = int32_t (state.SDST) != int32_t (state.IMM16);
}
void run_s_cmpk_lg_u32(WfStateSOPK& state) {
    state.SCC = state.SDST != state.IMM16;
}
void run_s_cmpk_lt_i32(WfStateSOPK& state) {
    state.SCC = int32_t(state.SDST) < int32_t(state.IMM16);
}
void run_s_cmpk_lt_u32(WfStateSOPK& state) {
    state.SCC = state.SDST < state.IMM16;
}
void run_s_getreg_b32(WfStateSOPK& state) {
    //todo
}
void run_s_movk_i32(WfStateSOPK& state) {
    state.SCC = int32_t (state.IMM16);
}
void run_s_mulk_i32(WfStateSOPK& state) {
    state.SDST = int32_t(state.SDST) * int32_t(state.IMM16);
}
void run_s_setreg_b32(WfStateSOPK& state) {
    //todo
}
void run_s_setreg_imm32_b32(WfStateSOPK& state) {
    //todo
}


void run_sopk(InstrKey instr, WfStateSOPK& state) {
    switch (instr) {
        case S_ADDK_I32:
            run_s_addk_i32(state);
            break;
        case S_CALL_B64:
            run_s_call_b64(state);
            break;
        case S_CBRANCH_I_FORK:
            run_s_cbranch_i_fork(state);
            break;
        case S_CMOVK_I32:
            run_s_cmovk_i32(state);
            break;
        case S_CMPK_EQ_I32:
            run_s_cmpk_eq_i32(state);
            break;
        case S_CMPK_EQ_U32:
            run_s_cmpk_eq_u32(state);
            break;
        case S_CMPK_GE_I32:
            run_s_cmpk_ge_i32(state);
            break;
        case S_CMPK_GE_U32:
            run_s_cmpk_ge_u32(state);
            break;
        case S_CMPK_GT_I32:
            run_s_cmpk_gt_i32(state);
            break;
        case S_CMPK_GT_U32:
            run_s_cmpk_gt_u32(state);
            break;
        case S_CMPK_LE_I32:
            run_s_cmpk_le_i32(state);
            break;
        case S_CMPK_LE_U32:
            run_s_cmpk_le_u32(state);
            break;
        case S_CMPK_LG_I32:
            run_s_cmpk_lg_i32(state);
            break;
        case S_CMPK_LG_U32:
            run_s_cmpk_lg_u32(state);
            break;
        case S_CMPK_LT_I32:
            run_s_cmpk_lt_i32(state);
            break;
        case S_CMPK_LT_U32:
            run_s_cmpk_lt_u32(state);
            break;
        case S_GETREG_B32:
            run_s_getreg_b32(state);
            break;
        case S_MOVK_I32:
            run_s_movk_i32(state);
            break;
        case S_MULK_I32:
            run_s_mulk_i32(state);
            break;
        case S_SETREG_B32:
            run_s_setreg_b32(state);
            break;
        case S_SETREG_IMM32_B32:
            run_s_setreg_imm32_b32(state);
            break;
        default:
            assert(false && "Unknown instruction met!");
            throw std::runtime_error(std::string("Unexpected instruction key: ") + get_instr_str(instr));
    }
}