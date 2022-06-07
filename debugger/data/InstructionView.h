#ifndef RED_O_LATOR_INSTRUCTIONVIEW_H
#define RED_O_LATOR_INSTRUCTIONVIEW_H

struct InstructionView {
    uint64_t address;
    std::string text;
    bool isLabel;

    explicit InstructionView(uint64_t address, std::string text, bool isLabel = false)
        : address{address}, text(std::move(text)), isLabel{isLabel} {}
};

#endif  // RED_O_LATOR_INSTRUCTIONVIEW_H
