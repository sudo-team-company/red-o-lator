#include "alu.h"

static inline void run_flat_store_dwordxn(WfStateFLAT& state, size_t wiInd, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        //todo
        Storage::get_instance()->write_data(state.VADDR[wiInd], 0, state.VDATA[wiInd * state.VDATA_SIZE_PER_WI + i]);
    }
}
static inline void run_flat_store_dword(WfStateFLAT& state, size_t wiInd) {
    run_flat_store_dwordxn(state, wiInd, 1);
}
static inline void run_flat_store_dwordx2(WfStateFLAT& state, size_t wiInd) {
    run_flat_store_dwordxn(state, wiInd, 2);
}
static inline void run_flat_store_dwordx3(WfStateFLAT& state, size_t wiInd) {
    run_flat_store_dwordxn(state, wiInd, 3);
}
static inline void run_flat_store_dwordx4(WfStateFLAT& state, size_t wiInd) {
    run_flat_store_dwordxn(state, wiInd, 4);
}

static inline void run_flat_store_short(WfStateFLAT& state, size_t wiInd) {
    assert(state.VDATA_SIZE_PER_WI == 1);
    //todo
    Storage::get_instance()->write_data(state.VADDR[wiInd], 0,
                                        static_cast<uint16_t>(state.VDATA[wiInd]));
}

void run_flat(const Instruction& instr, Wavefront* wf) {
    auto state = wf->get_flat_state(instr);
    for (size_t wiInd = 0; wiInd < wf->get_size(); ++wiInd) {
        if (!wf->work_item_masked(wiInd)) continue;

        switch (instr.get_key()) {
            case FLAT_STORE_DWORD:
                run_flat_store_dword(state, wiInd);
                break;
            case FLAT_STORE_DWORDX2:
                run_flat_store_dwordx2(state, wiInd);
                break;
            case FLAT_STORE_DWORDX3:
                run_flat_store_dwordx3(state, wiInd);
                break;
            case FLAT_STORE_DWORDX4:
                run_flat_store_dwordx4(state, wiInd);
                break;
            case FLAT_STORE_SHORT:
                run_flat_store_short(state, wiInd);
                break;
            default:
                UNSUPPORTED_INSTRUCTION("FLAT", get_instr_str(instr.get_key()));
        }
    }

    wf->update_with_flat_state(instr, state);
}