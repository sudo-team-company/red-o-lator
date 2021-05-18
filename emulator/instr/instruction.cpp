//
// Created by Diana Kudaiberdieva
//

#include "instruction.h"

Instruction* KernelCode::get_instr(uint64_t address) {
    assert(address % 4 && "Wrong instr address: not a multiple of 4");
    size_t index = address % 4;
    assert(index >= 0 && index < code.size() && "Wrong instr address: out of range");
    Instruction* instr = code[index].get();
    assert(instr != nullptr && "Wrong instruction address: instr is nullptr");
    return instr;
}
