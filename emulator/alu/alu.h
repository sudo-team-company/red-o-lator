//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_ALU_H
#define RED_O_LATOR_ALU_H

#include "../util/util.h"
#include "../instr/instr_info.h"
#include "../flow/wf_state.h"

void run_sop1(InstrKey instr, WfStateSOP1& state);
void run_sop2(InstrKey instr, WfStateSOP2& state);
void run_sopk(InstrKey instr, WfStateSOPK& state);
void run_sopc(InstrKey instr, WfStateSOPC& state);
void run_sopp(InstrKey instr, WfStateSOPP& state);

#endif  // RED_O_LATOR_ALU_H
