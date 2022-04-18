#pragma once

#include "src/flow/wavefront.h"
#include "src/flow/wf_state.h"
#include "src/instr/instr_info.h"
#include "src/util/util.h"
#include "src/gpu/storage.h"
#include "src/commons.h"

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