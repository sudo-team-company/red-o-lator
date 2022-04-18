//
// Created by Diana Kudaiberdieva
//

#include <stdexcept>
#include "alu.h"

static inline void run_s_absdiff_i32(WfStateSOP2& state) {
    state.SDST =
        std::abs(static_cast<int32_t>(state.SSRC0) - static_cast<int32_t>(state.SSRC1));
    state.SCC = state.SDST != 0;
}
static inline void run_s_addc_u32(WfStateSOP2& state) {
    uint64_t temp = state.SSRC0 + state.SSRC1;
    state.SDST = temp;
    state.SCC = temp >> 32;
}

static inline void run_s_add_i32(WfStateSOP2& state) {
    state.SDST = static_cast<int64_t>(state.SSRC0) + static_cast<int64_t>(state.SSRC1);
    state.SCC = (get_bit(31, state.SSRC0) == get_bit(31, state.SSRC1)) &&
                (get_bit(31, state.SSRC1) != get_bit(31, state.SDST));
}

static inline void run_s_add_u32(WfStateSOP2& state) {
    state.SDST = state.SSRC0 + state.SSRC1;
    state.SCC = state.SDST >> 32;
}

static inline void run_s_and_b32(WfStateSOP2& state) {
    state.SDST = state.SSRC0 & state.SSRC1;
    state.SCC = state.SDST != 0;
}

static inline void run_s_and_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 & state.SSRC1;
    state.SCC = state.SDST != 0;
}

static inline void run_s_andn2_b32(WfStateSOP2& state) {
    state.SDST = state.SSRC0 & ~state.SSRC1;
    state.SCC = state.SDST != 0;
}

static inline void run_s_andn2_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 & ~state.SSRC1;
    state.SCC = state.SDST != 0;
}

static inline void run_s_ashr_i32(WfStateSOP2& state) {
    state.SDST = static_cast<int32_t>(state.SSRC0) >> (state.SSRC1 & 31);
    state.SCC = state.SDST != 0;
}
static inline void run_s_ashr_i64(WfStateSOP2& state) {
    state.SDST = static_cast<int64_t>(state.SSRC0) >> (state.SSRC1 & 63);
    state.SCC = state.SDST != 0;
}
static inline void run_s_bfe_i32(WfStateSOP2& state) {
    uint8_t shift = state.SSRC1 & 31;
    uint8_t length = (state.SSRC1 >> 16) & 0x7f;
    if (length == 0) {
        state.SDST = 0;
    }
    if (shift + length < 32) {
        state.SDST =
            static_cast<int32_t>(state.SSRC0 << (32 - shift - length)) >> (32 - length);
    } else {
        state.SDST = static_cast<int32_t>(state.SSRC0) >> shift;
    }
    state.SCC = state.SDST != 0;
}

static inline void run_s_bfe_i64(WfStateSOP2& state) {
    uint8_t shift = state.SSRC1 & 63;
    uint8_t length = (state.SSRC1 >> 16) & 0x7f;
    if (length == 0) {
        state.SDST = 0;
    }
    if (shift + length < 64) {
        state.SDST =
            static_cast<int64_t>(state.SSRC0 << (64 - shift - length)) >> (64 - length);
    } else {
        state.SDST = static_cast<int64_t>(state.SSRC0) >> shift;
    }
    state.SCC = state.SDST != 0;
}

static inline void run_s_bfe_u32(WfStateSOP2& state) {
    uint8_t shift = state.SSRC1 & 31;
    uint8_t length = (state.SSRC1 >> 16) & 0x7f;
    if (length == 0) {
        state.SDST = 0;
    }
    if (shift + length < 32) {
        state.SDST = (state.SSRC0 << (32 - shift - length)) >> (32 - length);
    } else {
        state.SDST = state.SSRC0 >> shift;
    }
    state.SCC = state.SDST != 0;
}

static inline void run_s_bfe_u64(WfStateSOP2& state) {
    uint8_t shift = state.SSRC1 & 63;
    uint8_t length = (state.SSRC1 >> 16) & 0x7f;
    if (length == 0) {
        state.SDST = 0;
    }
    if (shift + length < 64) {
        state.SDST = (state.SSRC0 << (64 - shift - length)) >> (64 - length);
    } else {
        state.SDST = state.SSRC0 >> shift;
    }
    state.SCC = state.SDST != 0;
}
static inline void run_s_bfm_b32(WfStateSOP2& state) {
    state.SDST = ((uint32_t(1) << (state.SSRC0 & 31)) - 1) << (state.SSRC1 & 31);
}

static inline void run_s_bfm_b64(WfStateSOP2& state) {
    state.SDST = ((uint64_t(1) << (state.SSRC0 & 63)) - 1) << (state.SSRC1 & 63);
}

static inline void run_s_cbranch_g_fork(WfStateSOP2& state, Wavefront* wavefront) {
    uint64_t passes = (state.EXEC & state.SSRC0);
    uint64_t failures = (state.EXEC & ~state.SSRC0);
    uint8_t CSP = state.MODE->csp();
    if (passes == state.EXEC)
        state.PC->set_value(state.SSRC1);
    else if (failures == state.EXEC)
        state.PC->add(4);
    else if (bit_count(failures) < bit_count(passes)) {
        state.EXEC = failures;
        wavefront->set_sgpr_pair(CSP*4, passes);
        wavefront->set_sgpr_pair(CSP*4 + 2, state.SSRC1);
        state.MODE->csp(CSP + 1);
        state.PC->add(4);        /* jump to failure */
    } else {
        state.EXEC = passes;
        wavefront->set_sgpr_pair(CSP * 4, failures);
        wavefront->set_sgpr_pair(CSP * 4 + 2, state.PC->get_value() + 4);
        state.MODE->csp(CSP + 1);
        state.PC->set_value(state.SSRC1);     /* jump to passes */
    }
}

static inline void run_s_cselect_b32(WfStateSOP2& state) {
    state.SDST = state.SCC ? state.SSRC0 : state.SSRC1;
}

static inline void run_s_cselect_b64(WfStateSOP2& state) {
    state.SDST = state.SCC ? state.SSRC0 : state.SSRC1;
}

static inline void run_s_lshl_b32(WfStateSOP2& state) {
    state.SDST = static_cast<uint32_t>(state.SSRC0) << (state.SSRC1 & 31);
    state.SCC = state.SDST != 0;
}

static inline void run_s_lshl_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 << (state.SSRC1 & 63);
    state.SCC = state.SDST != 0;
}

static inline void run_s_lshl_n_add_u32(WfStateSOP2& state, size_t n) {
    uint64_t temp = (state.SSRC0 << n) + state.SSRC1;
    state.SDST = temp & 0xffffffff;
    state.SCC = temp >= (uint64_t(1) << 32);
}

static inline void run_s_lshl1_add_u32(WfStateSOP2& state) {
    run_s_lshl_n_add_u32(state, 1);
}

static inline void run_s_lshl2_add_u32(WfStateSOP2& state) {
    run_s_lshl_n_add_u32(state, 2);
}

static inline void run_s_lshl3_add_u32(WfStateSOP2& state) {
    run_s_lshl_n_add_u32(state, 3);
}

static inline void run_s_lshl4_add_u32(WfStateSOP2& state) {
    run_s_lshl_n_add_u32(state, 4);
}

static inline void run_s_lshr_b32(WfStateSOP2& state) {
    state.SDST = state.SSRC0 >> (state.SSRC1 & 31);
    state.SCC = state.SDST != 0;
}

static inline void run_s_lshr_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 >> (state.SSRC1 & 63);
    state.SCC = state.SDST != 0;
}

static inline void run_s_max_i32(WfStateSOP2& state) {
    auto SRC0 = static_cast<int32_t>(state.SSRC0);
    auto SRC1 = static_cast<int32_t>(state.SSRC1);
    state.SDST = std::max(SRC0, SRC1);
    state.SCC = SRC0 > SRC1;
}

static inline void run_s_max_u32(WfStateSOP2& state) {
    auto SRC0 = static_cast<uint32_t>(state.SSRC0);
    auto SRC1 = static_cast<uint32_t>(state.SSRC1);
    state.SDST = std::max(SRC0, SRC1);
    state.SCC = SRC0 > SRC1;
}

static inline void run_s_min_i32(WfStateSOP2& state) {
    auto SRC0 = static_cast<int32_t>(state.SSRC0);
    auto SRC1 = static_cast<int32_t>(state.SSRC1);
    state.SDST = std::min(SRC0, SRC1);
    state.SCC = SRC0 < SRC1;
}

static inline void run_s_min_u32(WfStateSOP2& state) {
    auto SRC0 = static_cast<uint32_t>(state.SSRC0);
    auto SRC1 = static_cast<uint32_t>(state.SSRC1);
    state.SDST = std::min(SRC0, SRC1);
    state.SCC = SRC0 < SRC1;
}

static inline void run_s_mul_hi_i32(WfStateSOP2& state) {
    state.SDST = (static_cast<int64_t>(state.SSRC0) * static_cast<int32_t>(state.SSRC1)) >> 32;
}

static inline void run_s_mul_hi_u32(WfStateSOP2& state) {
    state.SDST = (state.SSRC0 * state.SSRC1) >> 32;
}

static inline void run_s_mul_i32(WfStateSOP2& state) {
    state.SDST = static_cast<int32_t>(state.SSRC0) * int32_t(state.SSRC1);
}

static inline void run_s_nand_b32(WfStateSOP2& state) {
    state.SDST = ~(static_cast<uint32_t>(state.SSRC0) & static_cast<uint32_t>(state.SSRC1));
    state.SCC = state.SDST != 0;
}
static inline void run_s_nand_b64(WfStateSOP2& state) {
    state.SDST = ~(state.SSRC0 & state.SSRC1);
    state.SCC = state.SDST != 0;
}

static inline void run_s_nor_b32(WfStateSOP2& state) {
    state.SDST = ~(uint32_t(state.SSRC0) | uint32_t(state.SSRC1));
    state.SCC = state.SDST != 0;
}

static inline void run_s_nor_b64(WfStateSOP2& state) {
    state.SDST = ~(state.SSRC0 | state.SSRC1);
    state.SCC = state.SDST != 0;
}

static inline void run_s_or_b32(WfStateSOP2& state) {
    state.SDST = uint32_t(state.SSRC0) | uint32_t(state.SSRC1);
    state.SCC = state.SDST != 0;
}

static inline void run_s_or_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 | state.SSRC1;
    state.SCC = state.SDST != 0;
}

static inline void run_s_orn2_b32(WfStateSOP2& state) {
    state.SDST = uint32_t(state.SSRC0) | ~uint32_t(state.SSRC1);
    state.SCC = state.SDST != 0;
}

static inline void run_s_orn2_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 | ~state.SSRC1;
    state.SCC = state.SDST != 0;
}

static inline void run_s_pack_hh_b32_b16(WfStateSOP2& state) {
    state.SDST = (state.SSRC0 >> 16) | (state.SSRC1 & 0xffff0000);
}

static inline void run_s_pack_lh_b32_b16(WfStateSOP2& state) {
    state.SDST = (state.SSRC0 & 0xffff) | (state.SSRC1 & 0xffff0000);
}

static inline void run_s_pack_ll_b32_b16(WfStateSOP2& state) {
    state.SDST = (state.SSRC0 & 0xffff) | ((state.SSRC1 & 0xffff) << 16);
}

// Return from exception handler and continue. This instruction may
// only be used within a trap handler.
static inline void run_s_rfe_restore_b64(WfStateSOP2& state) {
    state.STATUS->priv(0);
    state.PC->set_value(state.SSRC0);
}

static inline void run_s_subb_u32(WfStateSOP2& state) {
    uint64_t temp = uint64_t(state.SSRC0) - uint64_t(state.SSRC1) - state.SCC;
    state.SDST = temp;
    state.SCC = (temp >> 32) & 1;
}

static inline void run_s_sub_i32(WfStateSOP2& state) {
    auto SSRC0 = int32_t(state.SSRC0);
    auto SSRC1 = int32_t(state.SSRC1);
    state.SDST = int32_t(SSRC0) - int32_t(SSRC1);
    state.SCC = (get_bit(31, SSRC0) != get_bit(31, SSRC1)) &&
                (get_bit(31, SSRC0) != get_bit(31, int32_t(state.SDST)));
}

static inline void run_s_sub_u32(WfStateSOP2& state) {
    uint64_t temp = uint64_t(state.SSRC0) - uint64_t(state.SSRC1);
    state.SDST = temp;
    state.SCC = (temp >> 32) != 0;
}

static inline void run_s_xnor_b32(WfStateSOP2& state) {
    state.SDST = ~(uint32_t(state.SSRC0) ^ uint32_t(state.SSRC1));
    state.SCC = state.SDST != 0;
}

static inline void run_s_xnor_b64(WfStateSOP2& state) {
    state.SDST = ~(state.SSRC0 ^ state.SSRC1);
    state.SCC = state.SDST != 0;
}

static inline void run_s_xor_b32(WfStateSOP2& state) {
    state.SDST = uint32_t(state.SSRC0) ^ uint32_t(state.SSRC1);
    state.SCC = state.SDST != 0;
}

static inline void run_s_xor_b64(WfStateSOP2& state) {
    state.SDST = state.SSRC0 ^ state.SSRC1;
    state.SCC = state.SDST != 0;
}

void run_sop2(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_sop2_state(instr);

    switch (instr.get_key()) {
        case S_CBRANCH_G_FORK:
            state.SSRC0 = to_uin64_t(wf->read_operand(*instr[0]));
            state.SSRC1 = to_uin64_t(wf->read_operand(*instr[1]));
            run_s_cbranch_g_fork(state, wf);
            break;
        case S_RFE_RESTORE_B64:
            state.SDST = to_uin64_t(wf->read_operand(*instr[0]));
            state.SSRC0 = to_uin64_t(wf->read_operand(*instr[1]));
            run_s_rfe_restore_b64(state);
            wf->write_operand_to_gpr(*instr[0], state.SDST);
            break;

        case S_ABSDIFF_I32:
            run_s_absdiff_i32(state);
            break;
        case S_ADDC_U32:
            run_s_addc_u32(state);
            break;
        case S_ADD_I32:
            run_s_add_i32(state);
            break;
        case S_ADD_U32:
            run_s_add_u32(state);
            break;
        case S_AND_B32:
            run_s_and_b32(state);
            break;
        case S_AND_B64:
            run_s_and_b64(state);
            break;
        case S_ANDN2_B32:
            run_s_andn2_b32(state);
            break;
        case S_ANDN2_B64:
            run_s_andn2_b64(state);
            break;
        case S_ASHR_I32:
            run_s_ashr_i32(state);
            break;
        case S_ASHR_I64:
            run_s_ashr_i64(state);
            break;
        case S_BFE_I32:
            run_s_bfe_i32(state);
            break;
        case S_BFE_I64:
            run_s_bfe_i64(state);
            break;
        case S_BFE_U32:
            run_s_bfe_u32(state);
            break;
        case S_BFE_U64:
            run_s_bfe_u64(state);
            break;
        case S_BFM_B32:
            run_s_bfm_b32(state);
            break;
        case S_BFM_B64:
            run_s_bfm_b64(state);
            break;
        case S_CSELECT_B32:
            run_s_cselect_b32(state);
            break;
        case S_CSELECT_B64:
            run_s_cselect_b64(state);
            break;
        case S_LSHL_B32:
            run_s_lshl_b32(state);
            break;
        case S_LSHL_B64:
            run_s_lshl_b64(state);
            break;
        case S_LSHL1_ADD_U32:
            run_s_lshl1_add_u32(state);
            break;
        case S_LSHL2_ADD_U32:
            run_s_lshl2_add_u32(state);
            break;
        case S_LSHL3_ADD_U32:
            run_s_lshl3_add_u32(state);
            break;
        case S_LSHL4_ADD_U32:
            run_s_lshl4_add_u32(state);
            break;
        case S_LSHR_B32:
            run_s_lshr_b32(state);
            break;
        case S_LSHR_B64:
            run_s_lshr_b64(state);
            break;
        case S_MAX_I32:
            run_s_max_i32(state);
            break;
        case S_MAX_U32:
            run_s_max_u32(state);
            break;
        case S_MIN_I32:
            run_s_min_i32(state);
            break;
        case S_MIN_U32:
            run_s_min_u32(state);
            break;
        case S_MUL_HI_I32:
            run_s_mul_hi_i32(state);
            break;
        case S_MUL_HI_U32:
            run_s_mul_hi_u32(state);
            break;
        case S_MUL_I32:
            run_s_mul_i32(state);
            break;
        case S_NAND_B32:
            run_s_nand_b32(state);
            break;
        case S_NAND_B64:
            run_s_nand_b64(state);
            break;
        case S_NOR_B32:
            run_s_nor_b32(state);
            break;
        case S_NOR_B64:
            run_s_nor_b64(state);
            break;
        case S_OR_B32:
            run_s_or_b32(state);
            break;
        case S_OR_B64:
            run_s_or_b64(state);
            break;
        case S_ORN2_B32:
            run_s_orn2_b32(state);
            break;
        case S_ORN2_B64:
            run_s_orn2_b64(state);
            break;
        case S_PACK_HH_B32_B16:
            run_s_pack_hh_b32_b16(state);
            break;
        case S_PACK_LH_B32_B16:
            run_s_pack_lh_b32_b16(state);
            break;
        case S_PACK_LL_B32_B16:
            run_s_pack_ll_b32_b16(state);
            break;
        case S_SUBB_U32:
            run_s_subb_u32(state);
            break;
        case S_SUB_I32:
            run_s_sub_i32(state);
            break;
        case S_SUB_U32:
            run_s_sub_u32(state);
            break;
        case S_XNOR_B32:
            run_s_xnor_b32(state);
            break;
        case S_XNOR_B64:
            run_s_xnor_b64(state);
            break;
        case S_XOR_B32:
            run_s_xor_b32(state);
            break;
        case S_XOR_B64:
            run_s_xor_b64(state);
            break;
        default:
            unsupported_instruction("SOP2", get_instr_str(instr.get_key()));
    }

    wf->update_with_sop2_state(instr, state);
}