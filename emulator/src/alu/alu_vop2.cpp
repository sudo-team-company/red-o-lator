#include "alu.h"

namespace {
void run_v_addc_u32(WfStateVOP2& state, size_t wiInd) {
    uint64_t mask = 1ull << wiInd;
    uint8_t CC = (state.VCC & mask) ? 1 : 0;
    uint64_t temp = (uint64_t) state.SRC0[wiInd] + (uint64_t) state.SRC1[wiInd] + CC;
    state.VDST[wiInd] = temp & 0xffffffff;
    if (temp >= 0x100000000ULL) {
        state.VCC |= mask;
    } else {
        state.VCC &= ~mask;
    }
}

void run_v_add_u32(WfStateVOP2& state, size_t wiInd) {
    auto TEMP = state.SRC0[wiInd] + state.SRC1[wiInd];
    state.VDST[wiInd] = state.CLAMP ? std::min(TEMP, 0xffffffffull) : TEMP;
}

void run_v_ashr_i32(WfStateVOP2& state, size_t wiInd) {
    state.VDST[wiInd] = (state.SRC0[wiInd] >> (state.SRC1[wiInd] & 31)) & 0xffffffffull;
}
void run_v_lshr_b32(WfStateVOP2& state, size_t wiInd) {
    state.VDST[wiInd] = (state.SRC0[wiInd] >> (state.SRC1[wiInd] & 31)) & 0xffffffffull;
}
void run_v_and_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = state.SRC0[i] & state.SRC1[i];
}

void run_v_ashrrev_i32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = (int32_t) state.SRC1[i] >> (state.SRC0[i] & 31);
}

void run_v_bcnt_u32_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = state.SRC1[i] + bit_count(state.SRC0[i]);
}

void run_v_bfm_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = ((1U << (state.SRC0[i] & 31)) - 1) << (state.SRC1[i] & 31);
}

void run_v_cndmask_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = ((state.VCC >> i) == 1 ? state.SRC1[i] : state.SRC0[i]) & 0xffffffff;
}

void run_v_cvt_pk_i16_i32(WfStateVOP2& state, size_t i) {
    int16_t D0 = std::max(std::min(int32_t(state.SRC0[i]), int32_t(0x7fff)), -0x8000);
    int16_t D1 = std::max(std::min(int32_t(state.SRC1[i]), int32_t(0x7fff)), -0x8000);
    state.VDST[i] = D0 | (((uint32_t) D1) << 16);
}

void run_v_cvt_pk_u16_u32(WfStateVOP2& state, size_t i) {
    uint16_t D0 = std::min(state.SRC0[i], 0xffffull);
    uint16_t D1 = std::min(state.SRC1[i], 0xffffull);
    state.VDST[i] = D0 | (((uint32_t) D1) << 16);
}

void run_v_lshlrev_b16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = (state.SRC1[i] << (state.SRC0[i] & 15)) & 0xffff;
}

void run_v_lshlrev_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = (state.SRC1[i] << (state.SRC0[i] & 31)) & 0xffffffffull;
}

void run_v_lshrrev_b16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = (state.SRC1[i] >> (state.SRC0[i] & 15)) & 0xffff;
}

void run_v_lshrrev_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = state.SRC1[i] >> (state.SRC0[i] & 31);
}

void run_v_max_i16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::max((int16_t) state.SRC0[i], (int16_t) state.SRC1[i]);
}

void run_v_max_i32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::max((int32_t) state.SRC0[i], (int32_t) state.SRC1[i]);
}

void run_v_max_u16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::max(state.SRC0[i] & 0xffff, state.SRC1[i] & 0xffff);
}

void run_v_max_u32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::max(state.SRC0[i], state.SRC1[i]);
}

void run_v_min_i16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::min((int16_t) state.SRC0[i], (int16_t) state.SRC1[i]);
}

void run_v_min_i32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::min((int32_t) state.SRC0[i], (int32_t) state.SRC1[i]);
}

void run_v_min_u16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::min(state.SRC0[i] & 0xffff, state.SRC1[i] & 0xffff);
}

void run_v_min_u32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = std::min(state.SRC0[i], state.SRC1[i]);
}

void run_v_mul_hi_i32_i24(WfStateVOP2& state, size_t i) {
    auto V0 = int32_t((state.SRC0[i] & 0x7fffff) |
                            (state.SRC0[i] & 0x800000 ? 0xff800000 : 0));
    auto V1 = int32_t((state.SRC1[i] & 0x7fffff) |
                         (state.SRC1[i] & 0x800000 ? 0xff800000 : 0));
    state.VDST[i] = ((int64_t) V0 * V1) >> 32;
}

void run_v_mul_hi_u32_u24(WfStateVOP2& state, size_t i) {
    state.VDST[i] =
        ((uint64_t) (state.SRC0[i] & 0xffffff) * (uint32_t) (state.SRC1[i] & 0xffffff)) >>
        32;
}

void run_v_mul_i32_i24(WfStateVOP2& state, size_t i) {
    auto V0 = int32_t((state.SRC0[i] & 0x7fffff) |
                            (state.SRC0[i] & 0x800000 ? 0xff800000 : 0));
    auto V1 = int32_t((state.SRC1[i] & 0x7fffff) |
                            (state.SRC1[i] & 0x800000 ? 0xff800000 : 0));
    state.VDST[i] = V0 * V1;
}

void run_v_mul_lo_u16(WfStateVOP2& state, size_t i) {
    state.VDST[i] = ((state.SRC0[i] & 0Xffff) * (state.SRC1[i] & 0xffff)) & 0xffff;
}

void run_v_mul_u32_u24(WfStateVOP2& state, size_t i) {
    state.VDST[i] =
        (uint32_t) (state.SRC0[i] & 0xffffff) * (uint32_t) (state.SRC1[i] & 0xffffff);
}

void run_v_or_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = state.SRC0[i] | state.SRC1[i];
}

void run_v_sub_u16(WfStateVOP2& state, size_t i) {
    auto temp = int32_t((state.SRC0[i] & 0xffffull) - (state.SRC1[i] & 0xffffull));
    state.VDST[i] = state.CLAMP ? std::max(temp, 0) : temp;
}

void run_v_sub_u32(WfStateVOP2& state, size_t i) {
    auto temp = int64_t(state.SRC0[i] - state.SRC1[i]);
    state.VDST[i] = state.CLAMP ? std::max(int64_t(0), temp) : temp;
}

void run_v_subrev_u16(WfStateVOP2& state, size_t i) {
    auto temp = int32_t((state.SRC1[i] & 0xffffull) - (state.SRC0[i] & 0xffffull));
    state.VDST[i] = state.CLAMP ? std::max(0, temp) : temp;
}

void run_v_subrev_u32(WfStateVOP2& state, size_t i) {
    auto temp = int64_t(state.SRC1[i] - state.SRC0[i]);
    state.VDST[i] = state.CLAMP ? std::max(int64_t(0), temp) : temp;
}

void run_v_xor_b32(WfStateVOP2& state, size_t i) {
    state.VDST[i] = state.SRC0[i] ^ state.SRC1[i];
}

void run_v_sub_f32(WfStateVOP2& state, size_t i) {
    auto VDST = to_float(state.SRC0[i]) - to_float(state.SRC1[i]);
    state.VDST[i] = to_uint64(VDST);
}
void run_v_mul_f32(WfStateVOP2& state, size_t i) {
    auto VDST = to_float(state.SRC0[i]) * to_float(state.SRC1[i]);
    state.VDST[i] = to_uint64(VDST);
}
void run_v_mac_f32(WfStateVOP2& state, size_t i) {
    auto VDST =
        to_float(state.SRC0[i]) * to_float(state.SRC1[i]) + to_float(state.VDST[i]);
    state.VDST[i] = to_uint64(VDST);
}
void run_v_add_f32(WfStateVOP2& state, size_t i) {
    auto VDST = to_float(state.SRC0[i]) + to_float(state.SRC1[i]);
    state.VDST[i] = to_uint64(VDST);
}
void run_v_max_f32(WfStateVOP2& state, size_t i) {
    auto VDST = std::max(to_float(state.SRC0[i]), to_float(state.SRC1[i]));
    state.VDST[i] = to_uint64(VDST);
}
void run_v_min_f32(WfStateVOP2& state, size_t i) {
    auto VDST = std::min(to_float(state.SRC0[i]), to_float(state.SRC1[i]));
    state.VDST[i] = to_uint64(VDST);
}
void run_v_subrev_f32(WfStateVOP2& state, size_t i) {
    auto VDST = to_float(state.SRC1[i]) - to_float(state.SRC0[i]);
    state.VDST[i] = to_uint64(VDST);
}
}  // namespace

void run_vop2(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vop2_state(instr);
    auto instrKey = instr.get_key();
    for (size_t wiInd = 0; wiInd < wf->get_size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instrKey) {
            case V_ADDC_U32: run_v_addc_u32(state, wiInd); break;
            case V_ADD_U32: run_v_add_u32(state, wiInd); break;
            case V_SUB_U32: run_v_sub_u32(state, wiInd); break;
            case V_AND_B32: run_v_and_b32(state, wiInd); break;
            case V_ASHRREV_I32: run_v_ashrrev_i32(state, wiInd); break;
            case V_BCNT_U32_B32: run_v_bcnt_u32_b32(state, wiInd); break;
            case V_BFM_B32: run_v_bfm_b32(state, wiInd); break;
            case V_CNDMASK_B32: run_v_cndmask_b32(state, wiInd); break;
            case V_CVT_PK_I16_I32: run_v_cvt_pk_i16_i32(state, wiInd); break;
            case V_CVT_PK_U16_U32: run_v_cvt_pk_u16_u32(state, wiInd); break;
            case V_LSHLREV_B16: run_v_lshlrev_b16(state, wiInd); break;
            case V_LSHLREV_B32: run_v_lshlrev_b32(state, wiInd); break;
            case V_LSHRREV_B16: run_v_lshrrev_b16(state, wiInd); break;
            case V_LSHRREV_B32: run_v_lshrrev_b32(state, wiInd); break;
            case V_MAX_I16: run_v_max_i16(state, wiInd); break;
            case V_MAX_I32: run_v_max_i32(state, wiInd); break;
            case V_MAX_U16: run_v_max_u16(state, wiInd); break;
            case V_MAX_U32: run_v_max_u32(state, wiInd); break;
            case V_MIN_I16: run_v_min_i16(state, wiInd); break;
            case V_MIN_I32: run_v_min_i32(state, wiInd); break;
            case V_MIN_U16: run_v_min_u16(state, wiInd); break;
            case V_MIN_U32: run_v_min_u32(state, wiInd); break;
            case V_MUL_HI_I32_I24: run_v_mul_hi_i32_i24(state, wiInd); break;
            case V_MUL_HI_U32_U24: run_v_mul_hi_u32_u24(state, wiInd); break;
            case V_MUL_I32_I24: run_v_mul_i32_i24(state, wiInd); break;
            case V_MUL_LO_U16: run_v_mul_lo_u16(state, wiInd); break;
            case V_MUL_U32_U24: run_v_mul_u32_u24(state, wiInd); break;
            case V_OR_B32: run_v_or_b32(state, wiInd); break;
            case V_SUB_U16: run_v_sub_u16(state, wiInd); break;
            case V_SUBREV_U16: run_v_subrev_u16(state, wiInd); break;
            case V_SUBREV_U32: run_v_subrev_u32(state, wiInd); break;
            case V_XOR_B32: run_v_xor_b32(state, wiInd); break;
            case V_SUB_F32: run_v_sub_f32(state, wiInd); break;
            case V_MUL_F32: run_v_mul_f32(state, wiInd); break;
            case V_MAC_F32: run_v_mac_f32(state, wiInd); break;
            case V_ADD_F32: run_v_add_f32(state, wiInd); break;
            case V_MAX_F32: run_v_max_f32(state, wiInd); break;
            case V_MIN_F32: run_v_min_f32(state, wiInd); break;
            case V_SUBREV_F32: run_v_subrev_f32(state, wiInd); break;
            case V_LSHR_B32: run_v_lshr_b32(state, wiInd); break;
            case V_ASHR_I32: run_v_ashr_i32(state, wiInd); break;
            default: UNSUPPORTED_INSTRUCTION("VOP2", get_mnemonic(instr.get_key()));
        }
    }
    wf->update_with_vop2_state(instr, state);
}
