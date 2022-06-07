#include "alu.h"

namespace {
void run_flat_store_dwordxn(WfStateFLATStore& state,
                                          size_t wiInd,
                                          size_t n) {
    for (size_t i = 0; i < n; ++i) {
        Storage::get_instance()->write_data(
            state.VADDR[wiInd], 0, state.VDATA[wiInd * state.vdataAmountPerWi + i]);
    }
}

void run_flat_store_short(WfStateFLATStore& state, size_t wiInd) {
    assert(state.vdataAmountPerWi == 1);
    Storage::get_instance()->write_data(state.VADDR[wiInd], 0,
                                        static_cast<uint16_t>(state.VDATA[wiInd]));
}

void run_flat_load_dwordxn(WfStateFLATLoad& state, size_t wiInd, size_t n) {
    assert(state.vdstAmountPerWi >= n);
    for (size_t i = 0; i < n; ++i) {
        auto data = Storage::get_instance()->read_4_bytes(state.VADDR[wiInd], 0);
        state.VDST[wiInd * state.vdstAmountPerWi + i] = data;
    }
}

std::function<void(WfStateFLATStore&, size_t)> execute_flat_store(
    const Instruction& instr) {
    using namespace std::placeholders;
    switch (instr.get_key()) {
        case FLAT_STORE_DWORD: return std::bind(run_flat_store_dwordxn, _1, _2, 1); //NOLINT
        case FLAT_STORE_DWORDX2: return std::bind(run_flat_store_dwordxn, _1, _2, 2); //NOLINT
        case FLAT_STORE_DWORDX3: return std::bind(run_flat_store_dwordxn, _1, _2, 3); //NOLINT
        case FLAT_STORE_DWORDX4: return std::bind(run_flat_store_dwordxn, _1, _2, 4); //NOLINT
        case FLAT_STORE_SHORT: return run_flat_store_short;
        default: UNSUPPORTED_INSTRUCTION("FLAT", get_mnemonic(instr.get_key()));
    }
}

std::function<void(WfStateFLATLoad&, size_t)> execute_flat_load(
    const Instruction& instr) {
    using namespace std::placeholders;
    switch (instr.get_key()) {
        case FLAT_LOAD_DWORD: return std::bind(run_flat_load_dwordxn, _1, _2, 1); //NOLINT
        case FLAT_LOAD_DWORDX2: return std::bind(run_flat_load_dwordxn, _1, _2, 2); //NOLINT
        case FLAT_LOAD_DWORDX3: return std::bind(run_flat_load_dwordxn, _1, _2, 3); //NOLINT
        case FLAT_LOAD_DWORDX4: return std::bind(run_flat_load_dwordxn, _1, _2, 4); //NOLINT
        default: UNSUPPORTED_INSTRUCTION("FLAT", get_mnemonic(instr.get_key()));
    }
}
}
void run_flat(const Instruction &instr, Wavefront *wf) {
    auto instrKey = instr.get_key();
    if (instrKey >= FLAT_STORE_DWORD && instrKey <= FLAT_STORE_SHORT) {
        auto state = wf->get_flat_store_state(instr);
        for (size_t wiInd = 0; wiInd < wf->get_size(); ++wiInd) {
            execute_flat_store(instr)(state, wiInd);
        }
    } else {
        auto state = wf->get_flat_load_state(instr);
        for (size_t wiInd = 0; wiInd < wf->get_size(); ++wiInd) {
            execute_flat_load(instr)(state, wiInd);
        }
        wf->update_with_flat_load_state(instr, state);
    }
}