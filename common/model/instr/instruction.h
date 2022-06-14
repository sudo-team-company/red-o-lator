#pragma once

#include <common/utils/common.hpp>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include "instr_info.h"
#include "reg_info.h"

enum OperandType { REGISTER, FLOAT_CONST, INT_CONST, LITERAL_CONST, LABEL };

struct Operand {
    OperandType type;
    std::variant<int, uint32_t, float, RegisterType> value;
    size_t regAmount = 0;

    explicit Operand(OperandType type,
                     std::variant<int, uint32_t, float, RegisterType> value,
                     size_t regAmount = 0);
};

struct Instruction {
    Instruction(uint32_t addr,
                std::string instr,
                std::string originalLine,
                const std::vector<Operand>& args);

    InstrKey get_key() const { return instrKey; }

    Operand* operator[](size_t index) {
        assert(index < operands.size());
        return operands[index].get();
    }

    Operand const* operator[](size_t index) const {
        assert(index < operands.size());
        return operands[index].get();
    }

    size_t get_operands_count() const noexcept { return operands.size(); }

    void set_width_and_format(size_t width) {
        widthInBytes = width;
        format = get_instr_format(instrKey, widthInBytes);
    }

    size_t get_width() const {
        if (widthInBytes != 4 && widthInBytes != 8) {
            throw std::runtime_error(std::string("Wrong instruction widthInBytes ") +
                                     std::to_string(widthInBytes) + " for " +
                                     get_mnemonic(instrKey));
        }
        return widthInBytes;
    }

    uint32_t get_addr() const {
        return addr;
    }

    const std::string& get_raw_instr() const { return rawInstruction; }

    const std::string& get_original_line() const { return originalLine; }

    InstrFormat get_format() const {
        return format;
    }

   private:
    InstrKey instrKey;
    InstrFormat format = UNDEFINED;
    size_t widthInBytes = 0;
    uint32_t addr;
    std::string rawInstruction;
    std::string originalLine;
    std::vector<std::unique_ptr<Operand>> operands;
};

struct Label {
    std::string name;
    uint32_t addr;

    explicit Label(std::string name, uint32_t addr);
};

struct KernelCode {
    explicit KernelCode();
    Instruction* get_instr(uint64_t) const;
    void add_instr(uint32_t addr,
                   std::string& instr,
                   const std::string& originalLine,
                   const std::vector<Operand>& args = {});
    void add_label(uint32_t addr, const std::string& name);

    std::unordered_map<std::uint64_t, std::unique_ptr<Instruction>> code = std::unordered_map<std::uint64_t, std::unique_ptr<Instruction>>();
    std::unordered_map<std::uint64_t, std::unique_ptr<Label>> labels = std::unordered_map<std::uint64_t, std::unique_ptr<Label>>();
};
