//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

void run_instr(const Instruction& instr, Wavefront* wf) {
    auto instrFormat = get_instr_format(instr.get_instr_key());
    switch (instrFormat) {
        case SOP1_FORMAT:


            break;
        case SOP2_FORMAT:
            break;
        case SOPK_FORMAT:
            break;
        case SOPP:
            break;
        case SMEM:
            break;
        case SOPC:
            break;
        case VOP1:
        case VOP2:
        case VOPC:
        case VINTRP:
        case VOP3A:
        case VOP3B:
        case VOP3P:
        case FLAT:
            assert("Unsupported instruction type");
    }
}