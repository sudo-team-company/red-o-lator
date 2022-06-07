#include "alu.h"

void run_instr(const Instruction& instr, Wavefront* wf) {
    auto instrFormat = instr.get_format();
    switch (instrFormat) {
        case SOP1: run_sop1(instr, wf); break;
        case SOP2: run_sop2(instr, wf); break;
        case SOPK: run_sopk(instr, wf); break;
        case SOPP: run_sopp(instr, wf); break;
        case SMEM: run_smem(instr, wf); break;
        case SOPC: run_sopc(instr, wf); break;
        case VOP1: run_vop1(instr, wf); break;
        case VOP2: run_vop2(instr, wf); break;
        case VOP3A:
        case VOP3B: run_vop3(instr, wf); break;
        case FLAT: run_flat(instr, wf); break;
        case VOPC: run_vopc(instr, wf); break;
        case VINTRP:
        case VOP3P:
        default:
            logger.error(std::string("Unsupported instruction type ") + std::to_string(instrFormat) + " for instruction: " + get_mnemonic(instr.get_key()));
            assert(false);
    }
}