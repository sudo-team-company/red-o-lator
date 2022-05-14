#include "alu.h"

static inline constexpr void run_F(WfStateVOPC& state, size_t i) {
    state.SDST &= ~(1ull << i);
}

static inline constexpr void run_TRU(WfStateVOPC& state, size_t i) {
    state.SDST |= (1ull << i);
}

template <typename T>
static inline constexpr void run_LT(WfStateVOPC& state, size_t i) {
    auto SRC0 = static_cast<T>(state.SRC0[i]);
    auto SRC1 = static_cast<T>(state.SRC1[i]);
    SRC0 < SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_EQ(WfStateVOPC& state, size_t i) {
    auto SRC0 = static_cast<T>(state.SRC0[i]);
    auto SRC1 = static_cast<T>(state.SRC1[i]);
    SRC0 == SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_LE(WfStateVOPC& state, size_t i) {
    auto SRC0 = static_cast<T>(state.SRC0[i]);
    auto SRC1 = static_cast<T>(state.SRC1[i]);
    SRC0 <= SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_GT(WfStateVOPC& state, size_t i) {
    auto SRC0 = static_cast<T>(state.SRC0[i]);
    auto SRC1 = static_cast<T>(state.SRC1[i]);
    SRC0 > SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_LG(WfStateVOPC& state, size_t i) {
    auto SRC0 = static_cast<T>(state.SRC0[i]);
    auto SRC1 = static_cast<T>(state.SRC1[i]);
    SRC0 != SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_GE(WfStateVOPC& state, size_t i) {
    auto SRC0 = static_cast<T>(state.SRC0[i]);
    auto SRC1 = static_cast<T>(state.SRC1[i]);
    SRC0 >= SRC1 ? run_TRU(state, i) : run_F(state, i);
}

void run_vopc(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vopc_state(instr);
    for (size_t i = 0; i < wf->get_size(); ++i) {
        if (!wf->work_item_masked(i)) continue;
        switch (instr.get_key()) {
            case V_CMPX_F_I16:
            case V_CMP_F_I16: run_F(state, i); break;
            case V_CMPX_LT_I16:
            case V_CMP_LT_I16: run_LT<int16_t>(state, i); break;
            case V_CMPX_EQ_I16:
            case V_CMP_EQ_I16: run_EQ<int16_t>(state, i); break;
            case V_CMPX_LE_I16:
            case V_CMP_LE_I16: run_LE<int16_t>(state, i); break;
            case V_CMPX_GT_I16:
            case V_CMP_GT_I16: run_GT<int16_t>(state, i); break;
            case V_CMPX_LG_I16:
            case V_CMP_LG_I16: run_LG<int16_t>(state, i); break;
            case V_CMPX_GE_I16:
            case V_CMP_GE_I16: run_GE<int16_t>(state, i); break;
            case V_CMPX_TRU_I16:
            case V_CMP_TRU_I16: run_TRU(state, i); break;
            case V_CMPX_F_U16:
            case V_CMP_F_U16: run_F(state, i); break;
            case V_CMPX_LT_U16:
            case V_CMP_LT_U16: run_LT<uint16_t>(state, i); break;
            case V_CMPX_EQ_U16:
            case V_CMP_EQ_U16: run_EQ<uint16_t>(state, i); break;
            case V_CMPX_LE_U16:
            case V_CMP_LE_U16: run_LE<uint16_t>(state, i); break;
            case V_CMPX_GT_U16:
            case V_CMP_GT_U16: run_GT<uint16_t>(state, i); break;
            case V_CMPX_LG_U16:
            case V_CMP_LG_U16: run_LG<uint16_t>(state, i); break;
            case V_CMPX_GE_U16:
            case V_CMP_GE_U16: run_GE<uint16_t>(state, i); break;
            case V_CMPX_TRU_U16:
            case V_CMP_TRU_U16: run_TRU(state, i); break;
            case V_CMPX_F_I32:
            case V_CMP_F_I32: run_F(state, i); break;
            case V_CMPX_LT_I32:
            case V_CMP_LT_I32: run_LT<int32_t>(state, i); break;
            case V_CMPX_EQ_I32:
            case V_CMP_EQ_I32: run_EQ<int32_t>(state, i); break;
            case V_CMPX_LE_I32:
            case V_CMP_LE_I32: run_LE<int32_t>(state, i); break;
            case V_CMPX_GT_I32:
            case V_CMP_GT_I32: run_GT<int32_t>(state, i); break;
            case V_CMPX_LG_I32:
            case V_CMP_LG_I32: run_LG<int32_t>(state, i); break;
            case V_CMPX_GE_I32:
            case V_CMP_GE_I32: run_GE<int32_t>(state, i); break;
            case V_CMPX_TRU_I32:
            case V_CMP_TRU_I32: run_TRU(state, i); break;
            case V_CMPX_F_U32:
            case V_CMP_F_U32: run_F(state, i); break;
            case V_CMPX_LT_U32:
            case V_CMP_LT_U32: run_LT<uint32_t>(state, i); break;
            case V_CMPX_EQ_U32:
            case V_CMP_EQ_U32: run_EQ<uint32_t>(state, i); break;
            case V_CMPX_LE_U32:
            case V_CMP_LE_U32: run_LE<uint32_t>(state, i); break;
            case V_CMPX_GT_U32:
            case V_CMP_GT_U32: run_GT<uint32_t>(state, i); break;
            case V_CMPX_LG_U32:
            case V_CMP_LG_U32: run_LG<uint32_t>(state, i); break;
            case V_CMPX_GE_U32:
            case V_CMP_GE_U32: run_GE<uint32_t>(state, i); break;
            case V_CMPX_TRU_U32:
            case V_CMP_TRU_U32: run_TRU(state, i); break;
            case V_CMPX_F_I64:
            case V_CMP_F_I64: run_F(state, i); break;
            case V_CMPX_LT_I64:
            case V_CMP_LT_I64: run_LT<int64_t>(state, i); break;
            case V_CMPX_EQ_I64:
            case V_CMP_EQ_I64: run_EQ<int64_t>(state, i); break;
            case V_CMPX_LE_I64:
            case V_CMP_LE_I64: run_LE<int64_t>(state, i); break;
            case V_CMPX_GT_I64:
            case V_CMP_GT_I64: run_GT<int64_t>(state, i); break;
            case V_CMPX_LG_I64:
            case V_CMP_LG_I64: run_LG<int64_t>(state, i); break;
            case V_CMPX_GE_I64:
            case V_CMP_GE_I64: run_GE<int64_t>(state, i); break;
            case V_CMPX_TRU_I64:
            case V_CMP_TRU_I64: run_TRU(state, i); break;
            case V_CMPX_F_U64:
            case V_CMP_F_U64: run_F(state, i); break;
            case V_CMPX_LT_U64:
            case V_CMP_LT_U64: run_LT<uint64_t>(state, i); break;
            case V_CMPX_EQ_U64:
            case V_CMP_EQ_U64: run_EQ<uint64_t>(state, i); break;
            case V_CMPX_LE_U64:
            case V_CMP_LE_U64: run_LE<uint64_t>(state, i); break;
            case V_CMPX_GT_U64:
            case V_CMP_GT_U64: run_GT<uint64_t>(state, i); break;
            case V_CMPX_LG_U64:
            case V_CMP_LG_U64: run_LG<uint64_t>(state, i); break;
            case V_CMPX_GE_U64:
            case V_CMP_GE_U64: run_GE<uint64_t>(state, i); break;
            case V_CMPX_TRU_U64:
            case V_CMP_TRU_U64: run_TRU(state, i); break;
            default: UNSUPPORTED_INSTRUCTION("VOPC", get_mnemonic(instr.get_key()));
        }
        if (utils::startsWith(instr.get_raw_instr(), "v_cmpx")) {
            if (get_bit(i, state.SDST)) {
                state.EXEC |= (1 << i);
            } else {
                state.EXEC &= ~(1 << i);
            }
        }
    }
}