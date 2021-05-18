//
// Created by Diana Kudaiberdieva
//

#include "compute_unit.h"

void ComputeUnit::runWorkGroup(WorkGroup* wg) {

}

void ComputeUnit::runWavefront(Wavefront* wavefront) {
    while (true) {
        Instruction* curInstr = wavefront->get_cur_instr();
        InstrKey instrKey = curInstr->get_instr_key();
        wavefront->to_next_instr();

        if (instrKey == S_ENDPGM) {
            wavefront->completed = true;
            break;
        }

        if (instrKey == S_BARRIER) {
            wavefront->atBarrier = true;
            break;
        }


    }
}
