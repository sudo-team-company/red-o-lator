#pragma once

#include <cmath>
#include "commons/commons.h"
#include "commons/util.h"
#include "flow/wavefront.h"
#include "flow/wf_state.h"
#include "gpu/storage.h"
#include "model/instr/instr_info.h"

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
void run_vopc(const Instruction&, Wavefront*);
void run_flat(const Instruction&, Wavefront*);