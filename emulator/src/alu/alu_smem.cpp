#include "alu.h"

static inline void run_s_load_dword_n(WfStateSMEM& state, uint32_t n) {
    state.SDST = to_uint32_v(Storage::get_instance()->read_data(state.BASE, state.OFFSET, n));
}

void run_smem(const Instruction& instr, Wavefront* wavefront) {
    auto state = wavefront->get_smem_state(instr);
    switch (instr.get_key()) {
        case S_LOAD_DWORD:
            run_s_load_dword_n(state, 4);
            break;
        case S_LOAD_DWORDX2:
            run_s_load_dword_n(state, 4 * 2);
            break;
        case S_LOAD_DWORDX4:
            run_s_load_dword_n(state, 4 * 4);
            break;
        case S_LOAD_DWORDX8:
            run_s_load_dword_n(state, 4 * 8);
            break;
        case S_LOAD_DWORDX16:
            run_s_load_dword_n(state, 4 * 16);
            break;
        default:
            UNSUPPORTED_INSTRUCTION("SMEM", get_mnemonic(instr.get_key()));
    }
    wavefront->update_with_smem_state(instr, state);
}
