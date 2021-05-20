//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

void run_s_load_dword_n(WfStateSMEM& state, uint32_t n) {
    state.SDST = to_uint32_v(Storage::get_instance()->read_data(state.BASE, state.OFFSET, n));
}

void run_s_load_dword(WfStateSMEM& state) {
    run_s_load_dword_n(state, 4);
}

void run_s_load_dword2(WfStateSMEM& state) {
    run_s_load_dword_n(state, 4 * 2);
}

void run_s_load_dword4(WfStateSMEM& state) {
    run_s_load_dword_n(state, 4 * 4);
}

void run_s_load_dword8(WfStateSMEM& state) {
    run_s_load_dword_n(state, 4 * 8);
}

void run_s_load_dword16(WfStateSMEM& state) {
    run_s_load_dword_n(state, 4 * 16);
}


