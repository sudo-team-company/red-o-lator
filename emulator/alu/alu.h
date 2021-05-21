//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_ALU_H
#define RED_O_LATOR_ALU_H

#include "../flow/wavefront.h"
#include "../flow/wf_state.h"
#include "../instr/instr_info.h"
#include "../util/util.h"
#include "../gpu/storage.h"

void run_instr(const Instruction& instr, Wavefront* wf);

void run_sop1(const Instruction&, Wavefront*);
void run_sop2(const Instruction&, Wavefront*);
void run_sopk(const Instruction&, Wavefront*);
void run_sopc(const Instruction&, Wavefront*);
void run_sopp(const Instruction&, Wavefront*);
void run_smem(const Instruction&, Wavefront*);
void run_vop1(const Instruction&, Wavefront*);
void run_vop2(const Instruction&, Wavefront*);
void run_vop3(const Instruction&, Wavefront*);
void run_flat(const Instruction&, Wavefront*);

#endif  // RED_O_LATOR_ALU_H
