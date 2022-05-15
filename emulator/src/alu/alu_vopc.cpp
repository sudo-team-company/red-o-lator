#include "alu.h"

float get(uint64_t value) {
    return to_float(value);
}

template <typename T>
T get(uint64_t value) {
    return static_cast<T>(value);
}

static inline constexpr void run_F(WfStateVOPC& state, size_t i) {
    state.SDST &= ~(1ull << i);
}

static inline constexpr void run_TRU(WfStateVOPC& state, size_t i) {
    state.SDST |= (1ull << i);
}

template <typename T>
static inline constexpr void run_LT(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    SRC0 < SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_EQ(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    SRC0 == SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_LE(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    SRC0 <= SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_GT(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    SRC0 > SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_LG(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    SRC0 != SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_GE(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    SRC0 >= SRC1 ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_NGE(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    !(SRC0 >= SRC1) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_NLT(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    !(SRC0 < SRC1) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_NEQ(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    !(SRC0 == SRC1) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_NLE(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    !(SRC0 <= SRC1) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_NGT(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    !(SRC0 > SRC1) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_NLG(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    !(SRC0 != SRC1) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_O(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    (!std::isnan(SRC0) && !std::isnan(SRC0)) ? run_TRU(state, i) : run_F(state, i);
}
template <typename T>
static inline constexpr void run_U(WfStateVOPC& state, size_t i) {
    auto SRC0 = get<T>(state.SRC0[i]);
    auto SRC1 = get<T>(state.SRC1[i]);
    (!std::isnan(SRC0) || !std::isnan(SRC0)) ? run_TRU(state, i) : run_F(state, i);
}
void run_vopc(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_vopc_state(instr);
    for (size_t i = 0; i < wf->get_size(); ++i) {
        if (!wf->work_item_masked(i)) continue;
        switch (instr.get_key()) {
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
            case V_CMP_LT_F32:
            case V_CMPX_LT_F32: run_LT<float>(state, i);  break; //todo test
            case V_CMP_EQ_F32:
            case V_CMPX_EQ_F32: run_EQ<float>(state, i);  break;
            case V_CMP_LE_F32:
            case V_CMPX_LE_F32: run_LE<float>(state, i);  break;
            case V_CMP_GT_F32:
            case V_CMPX_GT_F32: run_GT<float>(state, i);  break;
            case V_CMP_LG_F32:
            case V_CMPX_LG_F32: run_LG<float>(state, i);  break;
            case V_CMP_GE_F32:
            case V_CMPX_GE_F32: run_GE<float>(state, i);  break;
            case V_CMP_O_F32:
            case V_CMPX_O_F32: run_O<float>(state, i); break;
            case V_CMP_U_F32:
            case V_CMPX_U_F32: run_U<float>(state, i); break;
            case V_CMP_NGE_F32:
            case V_CMPX_NGE_F32: run_NGE<float>(state, i);  break;
            case V_CMP_NLG_F32:
            case V_CMPX_NLG_F32: run_NLG<float>(state, i);  break;
            case V_CMP_NGT_F32:
            case V_CMPX_NGT_F32: run_NGT<float>(state, i);  break;
            case V_CMP_NLE_F32:
            case V_CMPX_NLE_F32: run_NLE<float>(state, i);  break;
            case V_CMP_NEQ_F32:
            case V_CMPX_NEQ_F32: run_NEQ<float>(state, i);  break;
            case V_CMP_NLT_F32:
            case V_CMPX_NLT_F32: run_NLT<float>(state, i);  break;
            case V_CMPS_LT_F64:
            case V_CMPX_LT_F64: run_LT<double>(state, i);  break;
            case V_CMPS_EQ_F64:
            case V_CMPX_EQ_F64: run_EQ<double>(state, i);  break;
            case V_CMPS_LE_F64:
            case V_CMPX_LE_F64: run_LE<double>(state, i);  break;
            case V_CMPS_GT_F64:
            case V_CMPX_GT_F64: run_GT<double>(state, i);  break;
            case V_CMPS_LG_F64:
            case V_CMPX_LG_F64: run_LG<double>(state, i);  break;
            case V_CMPS_GE_F64:
            case V_CMPX_GE_F64: run_GE<double>(state, i);  break;
            case V_CMPS_O_F64:
            case V_CMPX_O_F64: run_O<double>(state, i); break;
            case V_CMPS_U_F64:
            case V_CMPX_U_F64: run_U<double>(state, i); break;
            case V_CMPS_NGE_F64:
            case V_CMPX_NGE_F64: run_NGE<double>(state, i);  break;
            case V_CMPS_NLG_F64:
            case V_CMPX_NLG_F64: run_NLG<double>(state, i);  break;
            case V_CMPS_NGT_F64:
            case V_CMPX_NGT_F64: run_NGT<double>(state, i);  break;
            case V_CMPS_NLE_F64:
            case V_CMPX_NLE_F64: run_NLE<double>(state, i);  break;
            case V_CMPS_NEQ_F64:
            case V_CMPX_NEQ_F64: run_NEQ<double>(state, i);  break;
            case V_CMPS_NLT_F64:
            case V_CMPX_NLT_F64: run_NLT<double>(state, i);  break;
            default: {
                if (instr.get_raw_instr().find("_f_") != std::string::npos) {
                    run_F(state, i);
                } else if (instr.get_raw_instr().find("_tru_") != std::string::npos ||
                            instr.get_raw_instr().find("_t_") != std::string::npos) {
                    run_TRU(state, i);
                } else {
                    UNSUPPORTED_INSTRUCTION("VOPC", get_mnemonic(instr.get_key()));
                }
            }
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