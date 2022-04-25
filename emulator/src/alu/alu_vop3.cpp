#include "alu.h"

static inline void run_v_add_i16(WfStateVOP3 &state, size_t i) {
    auto temp = (int16_t)(state.SRC0[i] & 0xffff) + (int16_t)(state.SRC1[i] & 0xffff);
    state.VDST[i] = temp & 0xffffu;
    //todo clamp
}

static inline void run_v_add_i32(WfStateVOP3 &state, size_t i) {
    auto temp = (int32_t)(state.SRC0[i] & 0xffffffff) + (int32_t)(state.SRC1[i] & 0xffffffff);
    state.VDST[i] = temp & 0xffffffffu;
    //todo clamp
}

static inline void run_v_addc_co_u32(WfStateVOP3 &state, size_t i) {
    uint64_t temp = state.SRC0[i] + state.SRC1[i];
    state.VDST[i] = temp & 0xfffffffful;
    if (temp >= 0x100000000ULL) {
        state.VCC |= (1ull << i);
    } else {
        state.VCC &= ~(1ull << i) ;
    }
}

static inline void run_v_add3_u32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC0[i] + state.SRC1[i] + state.SRC2[i];
}

static inline void run_v_add_lshl_u32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] + state.SRC1[i]) << (state.SRC2[i] & 31);
}

static inline void run_v_alignbit_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = ((((uint64_t) state.SRC0[i]) << 32) | state.SRC1[i]) >> (state.SRC2[i] & 31);
}

static inline void run_v_alignbyte_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = ((((uint64_t) state.SRC0[i]) << 32) | state.SRC1[i]) >> ((state.SRC2[i] & 3) * 8);
}

static inline void run_v_and_or_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] & state.SRC1[i]) | state.SRC2[i];
}

static inline void run_v_ashr_i64(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (int64_t) state.SRC0[i] >> (state.SRC1[i] & 63);
}

static inline void run_v_ashrrev_i64(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (int64_t) state.SRC0[i] >> (state.SRC0[i] & 63);
}

static inline void run_v_bcnt_u32_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC1[i] + bit_count(state.SRC0[i]);
}

static inline void run_v_bfe_i32(WfStateVOP3 &state, size_t i) {
    uint8_t shift = state.SRC1[i] & 31;
    uint8_t length = state.SRC2[i] & 31;
    if (length == 0) {
        state.VDST[i] = 0;
    }
    if (shift + length < 32) {
        state.VDST[i] = (int32_t) (state.SRC0[i] << (32 - shift - length)) >> (32 - length);
    } else {
        state.VDST[i] = (int32_t) state.SRC0[i] >> shift;
    }
}

static inline void run_v_bfe_u32(WfStateVOP3 &state, size_t i) {
    uint8_t shift = state.SRC1[i] & 31;
    uint8_t length = state.SRC2[i] & 31;
    if (length == 0) {
        state.VDST[i] = 0;
    }
    if (shift + length < 32) {
        state.VDST[i] = state.SRC0[i] << (32 - shift - length) >> (32 - length);
    } else {
        state.VDST[i] = state.SRC0[i] >> shift;
    }
}

static inline void run_v_bfi_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] & state.SRC1[i]) | (~state.SRC0[i] & state.SRC2[i]);
}

static inline void run_v_bfm_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = ((1U << (state.SRC0[i] & 31)) - 1) << (state.SRC1[i] & 31);
}

static inline void run_v_cvt_pk_i16_i32(WfStateVOP3 &state, size_t i) {
    int16_t D0 = std::max(std::min((int32_t) state.SRC0[i], 0x7fff), -0x8000);
    int16_t D1 = std::max(std::min((int32_t) state.SRC1[i], 0x7fff), -0x8000);
    state.VDST[i] = D0 | (((uint32_t) D1) << 16);
}

static inline void run_v_cvt_pk_u16_u32(WfStateVOP3 &state, size_t i) {
    uint16_t D0 = std::min((uint32_t) state.SRC0[i], (uint32_t) 0xfffful);
    uint16_t D1 = std::min((uint32_t) state.SRC1[i], (uint32_t) 0xffff);
    state.VDST[i] = D0 | (((uint32_t) D1) << 16);
}

static inline void run_v_lerp_u8(WfStateVOP3 &state, size_t i) {
    for (uint8_t j = 0; j < 4; j++) {
        uint8_t S0 = (state.SRC0[i] >> (j * 8)) & 0xff;
        uint8_t S1 = (state.SRC1[i] >> (j * 8)) & 0xff;
        uint8_t S2 = (state.SRC2[i] >> (j * 8)) & 1;
        state.VDST[i] = (state.VDST[i] & ~(255U << (j * 8))) | (((S0 + S1 + S2) >> 1) << (j * 8));
    }
}

static inline void run_v_lshl_add_u32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] << (state.SRC1[i] & 31)) + state.SRC2[i];
}

static inline void run_v_lshl_b64(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC0[i] << (state.SRC1[i] & 63);
}

static inline void run_v_lshl_or_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] << (state.SRC1[i] & 31)) | state.SRC2[i];
}

static inline void run_v_lshlrev_b64(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC1[i] << (state.SRC0[i] & 63);
}

static inline void run_v_lshr_b64(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC0[i] >> (state.SRC1[i] & 63);
}

static inline void run_v_lshrrev_b64(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC1[i] >> (state.SRC0[i] & 63);
}

static inline void run_v_mad_i16(WfStateVOP3 &state, size_t i) {
    uint32_t temp = (sign_ext((int16_t) state.SRC0[i]) * (int16_t) state.SRC1[i] + (int16_t) state.SRC2[i]);
    state.VDST[i] = temp & 0xffff;
    //todo support clamp
    //VDST = CLAMP ? MIN(MAX(temp), -32768), 32767) : temp&0xffff
}

static inline void run_v_mad_i32_i16(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (uint32_t) (sign_ext((int16_t) state.SRC0[i]) * (int16_t) state.SRC1[i]) + state.SRC2[i];
}

static inline void run_v_mad_i32_i24(WfStateVOP3 &state, size_t i) {
    auto V0 = (int32_t) ((state.SRC0[i] & 0x7fffff) | (state.SRC0[i] & 0x800000 ? 0xff800000 : 0));
    auto V1 = (int32_t) ((state.SRC1[i] & 0x7fffff) | (state.SRC1[i] & 0x800000 ? 0xff800000 : 0));
    state.VDST[i] = V0 * V1 + state.SRC2[i];
}

static inline void run_v_mad_legacy_i16(WfStateVOP3 &state, size_t i) {
    uint32_t temp = (sign_ext((int16_t) state.SRC0[i]) * (int16_t) state.SRC1[i] + (int16_t) state.SRC2[i]);
    state.VDST[i] = temp & 0xffff;
    //todo clamp
    //VDST = CLAMP ? MIN(MAX(temp), -32768), 32767) : temp&0xffff
}

static inline void run_v_mad_legacy_u16(WfStateVOP3 &state, size_t i) {
    uint32_t temp = ((uint16_t) state.SRC0[i] * (uint16_t) state.SRC1[i] + (uint16_t) state.SRC2[i]) & 0xffff;
    state.VDST[i] = temp & 0xffff;
    //todo clamp
    //VDST = CLAMP ? MIN(temp, 0xffff) : (temp&0xffff)
}

static inline void run_v_mad_u16(WfStateVOP3 &state, size_t i) {
    uint32_t temp = ((uint16_t) state.SRC0[i] * (uint16_t) state.SRC1[i] + (uint16_t) state.SRC2[i]) & 0xffff;
    state.VDST[i] = temp & 0xffff;
    //todo clamp
    //state.VDST[i] = CLAMP ? std::min(temp, 0xffff) : (temp & 0xffff);
}

static inline void run_v_mad_u32_u16(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (uint32_t) ((state.SRC0[i] & 0xffff) * (state.SRC1[i] & 0xffff)) + state.SRC2[i];
}

static inline void run_v_mad_u32_u24(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (uint32_t) (state.SRC0[i] & 0xffffff) * (uint32_t) (state.SRC1[i] & 0xffffff) + state.SRC2[i];
}

static inline void run_v_max3_i16(WfStateVOP3 &state, size_t i) {
    if ((int16_t) state.SRC2[i] > (int16_t) state.SRC0[i] && (int16_t) state.SRC2[i] > (int16_t) state.SRC1[i]) {
        state.VDST[i] = (uint16_t) state.SRC2[i];
    } else {
        state.VDST[i] = (uint16_t) std::max((int16_t) state.SRC1[i], (int16_t) state.SRC0[i]);
    }
}

static inline void run_v_max3_i32(WfStateVOP3 &state, size_t i) {
    if ((int32_t) state.SRC2[i] > (int32_t) state.SRC0[i] && (int32_t) state.SRC2[i] > (int32_t) state.SRC1[i]) {
        state.VDST[i] = state.SRC2[i];
    } else {
        state.VDST[i] = std::max((int32_t) state.SRC1[i], (int32_t) state.SRC0[i]);
    }
}

static inline void run_v_max3_u16(WfStateVOP3 &state, size_t i) {
    if ((uint16_t) state.SRC2[i] > (uint16_t) state.SRC0[i] && (uint16_t) state.SRC2[i] > (uint16_t) state.SRC1[i]) {
        state.VDST[i] = (uint16_t) state.SRC2[i];
    } else {
        state.VDST[i] = std::max((uint16_t) state.SRC1[i], (uint16_t) state.SRC0[i]);
    }
}

static inline void run_v_max3_u32(WfStateVOP3 &state, size_t i) {
    if (state.SRC2[i] > state.SRC0[i] && state.SRC2[i] > state.SRC1[i]) {
        state.VDST[i] = state.SRC2[i];
    } else {
        state.VDST[i] = std::max(state.SRC1[i], state.SRC0[i]);
    }
}

static inline void run_v_mbcnt_hi_u32_b32(WfStateVOP3 &state, size_t i) {
    uint64_t threadMask = (1ull << i) - 1;
    uint32_t mask = state.SRC0[i] & uint32_t(threadMask >> 32);
    state.VDST[i] = state.SRC1[i] + bit_count(mask);
}

static inline void run_v_mbcnt_lo_u32_b32(WfStateVOP3 &state, size_t i) {
    uint64_t threadMask = (1ull << i) - 1;
    uint32_t mask = state.SRC0[i] & uint32_t(threadMask & 0xfffffffful);
    state.VDST[i] = state.SRC1[i] + bit_count(mask);
}

static inline void run_v_med3_i16(WfStateVOP3 &state, size_t i) {
    auto S0 = (int16_t) state.SRC0[i];
    auto S1 = (int16_t) state.SRC1[i];
    auto S2 = (int16_t) state.SRC2[i];
    if ((S2 > S1 && S2 < S0) || (S2 < S1 && S2 > S0)) {
        state.VDST[i] = (uint16_t) S2;
    } else if ((S1 > S2 && S1 < S0) || (S1 < S2 && S1 > S0)) {
        state.VDST[i] = (uint16_t) S1;
    } else {
        state.VDST[i] = (uint16_t) S0;
    }
}

static inline void run_v_med3_i32(WfStateVOP3 &state, size_t i) {
    auto S0 = (int32_t) state.SRC0[i];
    auto S1 = (int32_t) state.SRC1[i];
    auto S2 = (int32_t) state.SRC2[i];
    if ((S2 > S1 && S2 < S0) || (S2 < S1 && S2 > S0))
        state.VDST[i] = S2;
    else if ((S1 > S2 && S1 < S0) || (S1 < S2 && S1 > S0))
        state.VDST[i] = S1;
    else
        state.VDST[i] = S0;
}

static inline void run_v_med3_u16(WfStateVOP3 &state, size_t i) {
    auto S0 = (uint16_t) state.SRC0[i];
    auto S1 = (uint16_t) state.SRC1[i];
    auto S2 = (uint16_t) state.SRC2[i];
    if ((S2 > S1 && S2 < S0) || (S2 < S1 && S2 > S0))
        state.VDST[i] = S2;
    else if ((S1 > S2 && S1 < S0) || (S1 < S2 && S1 > S0))
        state.VDST[i] = S1;
    else
        state.VDST[i] = S0;
}

static inline void run_v_med3_u32(WfStateVOP3 &state, size_t i) {
    if ((state.SRC2[i] > state.SRC1[i] && state.SRC2[i] < state.SRC0[i]) ||
        (state.SRC2[i] < state.SRC1[i] && state.SRC2[i] > state.SRC0[i]))
        state.VDST[i] = state.SRC2[i];
    else if ((state.SRC1[i] > state.SRC2[i] && state.SRC1[i] < state.SRC0[i]) ||
             (state.SRC1[i] < state.SRC2[i] && state.SRC1[i] > state.SRC0[i]))
        state.VDST[i] = state.SRC1[i];
    else
        state.VDST[i] = state.SRC0[i];
}

static inline void run_v_min3_i16(WfStateVOP3 &state, size_t i) {
    if ((int16_t) state.SRC2[i] < (int16_t) state.SRC0[i] && (int16_t) state.SRC2[i] < (int16_t) state.SRC1[i])
        state.VDST[i] = (uint16_t) state.SRC2[i];
    else
        state.VDST[i] = (uint16_t) std::min((int16_t) state.SRC1[i], (int16_t) state.SRC0[i]);
}

static inline void run_v_min3_i32(WfStateVOP3 &state, size_t i) {
    if ((int32_t) state.SRC2[i] < (int32_t) state.SRC0[i] && (int32_t) state.SRC2[i] < (int32_t) state.SRC1[i])
        state.VDST[i] = state.SRC2[i];
    else
        state.VDST[i] = std::min((int32_t) state.SRC1[i], (int32_t) state.SRC0[i]);
}

static inline void run_v_min3_u16(WfStateVOP3 &state, size_t i) {
    if ((uint16_t) state.SRC2[i] < (uint16_t) state.SRC0[i] && (uint16_t) state.SRC2[i] < (uint16_t) state.SRC1[i])
        state.VDST[i] = (uint16_t) state.SRC2[i];
    else
        state.VDST[i] = std::min((uint16_t) state.SRC1[i], (uint16_t) state.SRC0[i]);
}

static inline void run_v_min3_u32(WfStateVOP3 &state, size_t i) {
    if (state.SRC2[i] < state.SRC0[i] && state.SRC2[i] < state.SRC1[i])
        state.VDST[i] = state.SRC2[i];
    else
        state.VDST[i] = std::min(state.SRC1[i], state.SRC0[i]);
}

//todo V_MQSAD_U32_U8

static inline void run_v_msad_u8(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC2[i];
    for (uint8_t j = 0; j < 4; j++) {
        if (((state.SRC1[i] >> (j * 8)) & 0xff) != 0) {
            int8_t diff = (uint8_t)((state.SRC0[i] >> (j * 8)) & 0xff) - (uint8_t)((state.SRC1[i] >> (j * 8)) & 0xff);
            state.VDST[i] += std::abs(diff);
        }
    }
}

static inline void run_v_mul_hi_i32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = ((int64_t) state.SRC0[i] * (int32_t) state.SRC1[i]) >> 32;
}

static inline void run_v_mul_hi_u32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = ((uint64_t) state.SRC0[i] * state.SRC1[i]) >> 32;
}

static inline void run_v_mul_lo_i32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (int32_t) state.SRC0[i] * (int32_t) state.SRC1[i];
}

static inline void run_v_or3_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC0[i] | state.SRC1[i] | state.SRC2[i];
}

static inline void run_v_pack_b32_f16(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] & 0xffff) | (state.SRC1[i] << 16);
}

static inline void run_v_perm_b32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = 0;
    uint64_t qword = (((uint64_t) state.SRC0[i]) << 32) | state.SRC1[i];
    for (uint8_t j = 0; j < 4; j++) {
        uint8_t choice = (state.SRC2[i] >> (8 * j)) & 0xff;
        uint8_t result;
        if (choice >= 13)
            result = 0xff;
        else if (choice == 12)
            result = 0;
        else if (choice >= 8)
            result = 0xff * qword >> ((choice - 8) * 16 + 15);
        else
            result = (qword >> (choice * 8)) & 0xff;
        state.VDST[i] |= (result << (j * 8));
    }
}

static inline void run_v_sad_hi_u8(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC2[i];
    for (uint8_t j = 0; j < 4; j++) {
        uint32_t arg1 = (state.SRC0[i] >> (j * 8)) & 0xff;
        uint32_t arg2 = (state.SRC1[i] >> (j * 8)) & 0xff;
        auto diff = int32_t (arg1 - arg2);
        state.VDST[i] += std::abs(diff) << 16;
    }
}

static inline void run_v_sad_u16(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC2[i];
    auto diff1 = int32_t(state.SRC0[i] & 0xffff) - int32_t(state.SRC1[i] & 0xffff);
    state.VDST[i] += std::abs(diff1);
    auto diff2 = int32_t(state.SRC0[i] >> 16) - int32_t(state.SRC1[i] >> 16);
    state.VDST[i] += std::abs(diff2);
}

static inline void run_v_sad_u32(WfStateVOP3 &state, size_t i) {
    int32_t diff = state.SRC0[i] - state.SRC1[i];
    state.VDST[i] = state.SRC2[i] + std::abs(diff);
}

static inline void run_v_sad_u8(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC2[i];
    for (uint8_t j = 0; j < 4; j++) {
        auto value = ((int32_t) (state.SRC0[i] >> (j * 8)) & 0xff) - ((int32_t) (state.SRC1[i] >> (j * 8)) & 0xff);
        state.VDST[i] += std::abs(value);
    }
}

static inline void run_v_sub_i16(WfStateVOP3 &state, size_t i) {
    int16_t diff = int16_t (state.SRC0[i] & 0xffff) - int16_t (state.SRC1[i] & 0xffff);
    state.VDST[i] = diff;
    //todo clamp
}

static inline void run_v_sub_i32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC0[i] - state.SRC1[i];
//todo clamp
}

static inline void run_v_xad_u32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = (state.SRC0[i] ^ state.SRC1[i]) + state.SRC2[i];
}

static inline void run_v_mul_lo_u32(WfStateVOP3 &state, size_t i) {
    state.VDST[i] = state.SRC0[i] * state.SRC1[i];
}

void run_vop3(const Instruction &instr, Wavefront *wf) {
    auto state = wf->get_vop3_state(instr);
    for (size_t wiInd = 0; wiInd < wf->get_size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;
        switch (instr.get_key()) {
            case V_ADD_I16:
                run_v_add_i16(state, wiInd);
                break;
            case V_ADD_I32:
                run_v_add_i32(state, wiInd);
                break;
            case V_ADD3_U32:
                run_v_add3_u32(state, wiInd);
                break;
            case V_ADD_LSHL_U32:
                run_v_add_lshl_u32(state, wiInd);
                break;
            case V_ALIGNBIT_B32:
                run_v_alignbit_b32(state, wiInd);
                break;
            case V_ALIGNBYTE_B32:
                run_v_alignbyte_b32(state, wiInd);
                break;
            case V_AND_OR_B32:
                run_v_and_or_b32(state, wiInd);
                break;
            case V_ASHR_I64:
                run_v_ashr_i64(state, wiInd);
                break;
            case V_ASHRREV_I64:
                run_v_ashrrev_i64(state, wiInd);
                break;
            case V_BCNT_U32_B32:
                run_v_bcnt_u32_b32(state, wiInd);
                break;
            case V_BFE_I32:
                run_v_bfe_i32(state, wiInd);
                break;
            case V_BFE_U32:
                run_v_bfe_u32(state, wiInd);
                break;
            case V_BFI_B32:
                run_v_bfi_b32(state, wiInd);
                break;
            case V_BFM_B32:
                run_v_bfm_b32(state, wiInd);
                break;
            case V_CVT_PK_I16_I32:
                run_v_cvt_pk_i16_i32(state, wiInd);
                break;
            case V_CVT_PK_U16_U32:
                run_v_cvt_pk_u16_u32(state, wiInd);
                break;
            case V_LERP_U8:
                run_v_lerp_u8(state, wiInd);
                break;
            case V_LSHL_ADD_U32:
                run_v_lshl_add_u32(state, wiInd);
                break;
            case V_LSHL_B64:
                run_v_lshl_b64(state, wiInd);
                break;
            case V_LSHL_OR_B32:
                run_v_lshl_or_b32(state, wiInd);
                break;
            case V_LSHLREV_B64:
                run_v_lshlrev_b64(state, wiInd);
                break;
            case V_LSHR_B64:
                run_v_lshr_b64(state, wiInd);
                break;
            case V_LSHRREV_B64:
                run_v_lshrrev_b64(state, wiInd);
                break;
            case V_MAD_I16:
                run_v_mad_i16(state, wiInd);
                break;
            case V_MAD_I32_I16:
                run_v_mad_i32_i16(state, wiInd);
                break;
            case V_MAD_I32_I24:
                run_v_mad_i32_i24(state, wiInd);
                break;
            case V_MAD_LEGACY_I16:
                run_v_mad_legacy_i16(state, wiInd);
                break;
            case V_MAD_LEGACY_U16:
                run_v_mad_legacy_u16(state, wiInd);
                break;
            case V_MAD_U16:
                run_v_mad_u16(state, wiInd);
                break;
            case V_MAD_U32_U16:
                run_v_mad_u32_u16(state, wiInd);
                break;
            case V_MAD_U32_U24:
                run_v_mad_u32_u24(state, wiInd);
                break;
            case V_MAX3_I16:
                run_v_max3_i16(state, wiInd);
                break;
            case V_MAX3_I32:
                run_v_max3_i32(state, wiInd);
                break;
            case V_MAX3_U16:
                run_v_max3_u16(state, wiInd);
                break;
            case V_MAX3_U32:
                run_v_max3_u32(state, wiInd);
                break;
            case V_MBCNT_HI_U32_B32:
                run_v_mbcnt_hi_u32_b32(state, wiInd);
                break;
            case V_MBCNT_LO_U32_B32:
                run_v_mbcnt_lo_u32_b32(state, wiInd);
                break;
            case V_MED3_I16:
                run_v_med3_i16(state, wiInd);
                break;
            case V_MED3_I32:
                run_v_med3_i32(state, wiInd);
                break;
            case V_MED3_U16:
                run_v_med3_u16(state, wiInd);
                break;
            case V_MED3_U32:
                run_v_med3_u32(state, wiInd);
                break;
            case V_MIN3_I16:
                run_v_min3_i16(state, wiInd);
                break;
            case V_MIN3_I32:
                run_v_min3_i32(state, wiInd);
                break;
            case V_MIN3_U16:
                run_v_min3_u16(state, wiInd);
                break;
            case V_MIN3_U32:
                run_v_min3_u32(state, wiInd);
                break;
            case V_MSAD_U8:
                run_v_msad_u8(state, wiInd);
                break;
            case V_MUL_HI_I32:
                run_v_mul_hi_i32(state, wiInd);
                break;
            case V_MUL_HI_U32:
                run_v_mul_hi_u32(state, wiInd);
                break;
            case V_MUL_LO_I32:
                run_v_mul_lo_i32(state, wiInd);
                break;
            case V_OR3_B32:
                run_v_or3_b32(state, wiInd);
                break;
            case V_PACK_B32_F16:
                run_v_pack_b32_f16(state, wiInd);
                break;
            case V_PERM_B32:
                run_v_perm_b32(state, wiInd);
                break;
            case V_SAD_HI_U8:
                run_v_sad_hi_u8(state, wiInd);
                break;
            case V_SAD_U16:
                run_v_sad_u16(state, wiInd);
                break;
            case V_SAD_U32:
                run_v_sad_u32(state, wiInd);
                break;
            case V_SAD_U8:
                run_v_sad_u8(state, wiInd);
                break;
            case V_SUB_I16:
                run_v_sub_i16(state, wiInd);
                break;
            case V_SUB_I32:
                run_v_sub_i32(state, wiInd);
                break;
            case V_XAD_U32:
                run_v_xad_u32(state, wiInd);
                break;
            case V_MUL_LO_U32:
                run_v_mul_lo_u32(state, wiInd);
                break;
            case V_ADDC_CO_U32:
                run_v_addc_co_u32(state, wiInd);
                break;
            default:
                UNSUPPORTED_INSTRUCTION("VOP3", get_instr_str(instr.get_key()));
        }
    }
    wf->update_with_vop3_state(instr, state);
}
