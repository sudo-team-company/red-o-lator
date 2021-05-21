//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

static inline void run_s_addk_i32(WfStateSOPK& state) {
    state.SDST = static_cast<int32_t>(state.SDST) + static_cast<int32_t>(state.IMM16);
    int64_t temp = static_cast<int64_t>(state.SDST) + static_cast<int64_t>(state.IMM16);
    state.SCC = get_bit(31, temp) == get_bit(15, state.IMM16) &&
                get_bit(31, temp) != get_bit(31, static_cast<int32_t>(state.SDST));
}
static inline void run_s_call_b64(WfStateSOPK& state) {
    state.SDST = state.PC->get_value() + 4;
    state.PC->set_value(state.RELADDR);
}
static inline void run_s_cbranch_i_fork(WfStateSOPK& state) {
    // todo
}
static inline void run_s_cmovk_i32(WfStateSOPK& state) {
    if (state.SCC) state.SDST = static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_eq_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.SDST) == static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_eq_u32(WfStateSOPK& state) {
    state.SCC = state.SDST == state.IMM16;
}
static inline void run_s_cmpk_ge_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.SDST) >= static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_ge_u32(WfStateSOPK& state) {
    state.SCC = state.SDST >= state.IMM16;
}
static inline void run_s_cmpk_gt_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.SDST) >= static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_gt_u32(WfStateSOPK& state) {
    state.SCC = state.SDST > state.IMM16;
}
static inline void run_s_cmpk_le_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.SDST) <= static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_le_u32(WfStateSOPK& state) {
    state.SCC = state.SDST <= state.IMM16;
}
static inline void run_s_cmpk_lg_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.SDST) != static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_lg_u32(WfStateSOPK& state) {
    state.SCC = state.SDST != state.IMM16;
}
static inline void run_s_cmpk_lt_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.SDST) < static_cast<int32_t>(state.IMM16);
}
static inline void run_s_cmpk_lt_u32(WfStateSOPK& state) {
    state.SCC = state.SDST < state.IMM16;
}
static inline void run_s_getreg_b32(WfStateSOPK& state) {
    // todo
}
static inline void run_s_movk_i32(WfStateSOPK& state) {
    state.SCC = static_cast<int32_t>(state.IMM16);
}
static inline void run_s_mulk_i32(WfStateSOPK& state) {
    state.SDST = static_cast<int32_t>(state.SDST) * static_cast<int32_t>(state.IMM16);
}
static inline void run_s_setreg_b32(WfStateSOPK& state) {
    // todo
}
static inline void run_s_setreg_imm32_b32(WfStateSOPK& state) {
    // todo
}

void run_sopk(const Instruction& instruction, Wavefront* wf) {
    auto instrKey = instruction.get_key();

    // S_CALL_B64 SDST(2), RELADDR
    //  S_CBRANCH_I_FORK SRC0(2), RELADDR
    // S_GETREG_B32 SDST, HWREG(HWREGNAME, BITOFFSET, BITSIZE)
    // S_SETREG_B32 HWREG(HWREGNAME, BITOFFSET, BITSIZE), SDST
    // S_SETREG_IMM32_B32 HWREG(HWREGNAME, BITOFFSET, BITSIZE), IMM32

    switch (instrKey) {
        case S_CALL_B64: {
            auto state = wf->get_sopk_state();
            state.SDST = to_uin64_t(wf->read_operand(*instruction[0]));
            state.RELADDR = to_uin64_t(wf->read_operand(*instruction[1]));
            run_s_call_b64(state);
            wf->write_operand_to_gpr(*instruction[0], state.SDST);
            return;
        }
        case S_CBRANCH_I_FORK: {
            auto state = wf->get_sopk_state();
            state.SSRC0 = to_uin64_t(wf->read_operand(*instruction[0]));
            state.RELADDR = to_uin64_t(wf->read_operand(*instruction[1]));
            run_s_call_b64(state);
            return;
        }
        case S_GETREG_B32:
            //run_s_getreg_b32(state);
            return;
        case S_SETREG_B32:
            //run_s_setreg_b32(state);
            return;
        case S_SETREG_IMM32_B32:
            //run_s_setreg_imm32_b32(state);
            return;
    }

    auto state = wf->get_sopk_state(instruction);

    switch (instrKey) {
        case S_ADDK_I32:
            run_s_addk_i32(state);
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
        case S_MOVK_I32:
            run_s_movk_i32(state);
            break;
        case S_MULK_I32:
            run_s_mulk_i32(state);
            break;
        default:
            assert(false && "Unknown instruction met!");
            throw std::runtime_error(std::string("Unexpected instruction key: ") +
                                     get_instr_str(instrKey));
    }
    wf->update_with_sopk_state(instruction, state);
}