//
// Created by Diana Kudaiberdieva
//

#include "alu.h"

void run_instr(const Instruction& instr, Wavefront* wf) {
    auto instrFormat = get_instr_format(instr.get_key());
    switch (instrFormat) {
        case SOP1:
            run_sop1(instr, wf);
            break;
        case SOP2:
            run_sop2(instr, wf);
            break;
        case SOPK:
            run_sopk(instr, wf);
            break;
        case SOPP:
            run_sopp(instr, wf);
            break;
        case SMEM:
            run_smem(instr, wf);
            break;
        case SOPC:
            run_sopc(instr, wf);
            break;
        case VOP1:
            run_vop1(instr, wf);
            break;
        case VOP2:
            run_vop2(instr, wf);
            break;
        case VOPC:
        case VINTRP:
        case VOP3A:
        case VOP3B:
        case VOP3P:
        case FLAT:
        default:
            assert(false && "Unsupported instruction type");
    }
}