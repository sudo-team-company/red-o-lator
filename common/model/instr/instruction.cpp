#include "instruction.h"

#include <utility>

KernelCode::KernelCode() = default;

Instruction* KernelCode::get_instr(uint64_t address) const {
    assert(address % 4 == 0 && "Wrong instr address: not a multiple of 4");
    auto instrIter = code.find(address);
    assert(instrIter != code.end() && "Wrong instruction address: instr is nullptr");
    return instrIter->second.get();
}

void KernelCode::add_instr(uint32_t addr,
                           std::string& instr,
                           const std::string& originalLine,
                           const std::vector<Operand>& args) {
    code[addr] = std::make_unique<Instruction>(Instruction{addr, instr, originalLine, args});
}

void KernelCode::add_label(uint32_t addr,
                           const std::string& name) {
    labels[addr] = std::make_unique<Label>(Label{name, addr});
}

Instruction::Instruction(uint32_t addr,
                         std::string instr,
                         std::string originalLine,
                         const std::vector<Operand>& args)
    : addr(addr),
      rawInstruction(std::move(instr)),
      originalLine(std::move(originalLine)) {
    instrKey = get_instr_key(rawInstruction);
    operands = std::vector<std::unique_ptr<Operand>>();
    for (auto& arg : args) {
        operands.push_back(std::make_unique<Operand>(arg));
    }
}

Operand::Operand(OperandType type,
                  std::variant<int, uint32_t, float, RegisterType> value,
                  size_t regAmount) : type{type}, value{value}, regAmount{regAmount} {}

Label::Label(std::string name, uint32_t addr) : name{std::move(name)}, addr{addr} {}
