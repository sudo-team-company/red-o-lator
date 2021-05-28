#ifndef RED_O_LATOR_INSTRUCTION_H
#define RED_O_LATOR_INSTRUCTION_H

struct Instruction {
    uint64_t address;
    std::string text;
    bool isLabel = false;
};

#endif  // RED_O_LATOR_INSTRUCTION_H
