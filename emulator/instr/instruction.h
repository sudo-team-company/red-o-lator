//
// Created by Diana Kudaiberdieva
//

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "../reg/reg_info.h"
#include "instr_info.h"


enum OperandType { REGISTER, FLOAT, INT_CONST, LITERAL_CONST };

struct Operand {
    OperandType type;
    std::variant<int16_t, uint32_t, float, RegisterType> value;
    int waitcnt;
    size_t regAmount = 0;

    explicit Operand(int16_t i) : type(INT_CONST), value(i) {}
    explicit Operand(uint32_t u) : type(LITERAL_CONST), value(u) {}
    explicit Operand(float f) : type(FLOAT), value(f) {}
    explicit Operand(RegisterType regs, size_t n)
        : type(REGISTER), value(regs), regAmount(n) {}

    OperandType get_type() const {
        return type;
    }

    size_t get_reg_amount() const {
        return regAmount;
    }
};

struct Instruction {
    InstrKey get_instr_key() const {
        return instrKey;
    }

    Operand* operator[](size_t index) {
        return operands[index].get();
    }

    Operand const* operator[](size_t index) const {
        return operands[index].get();
    }

    size_t get_operands_count() const noexcept {
        return operands.size();
    }

    uint32_t get_addr() const {
        return addr;
    };

   private:
    InstrKey instrKey;
    std::vector<std::unique_ptr<Operand>> operands;
    uint32_t addr;
};

struct KernelCode {
    Instruction* get_instr(uint64_t);

   private:
    std::vector<std::unique_ptr<Instruction>> code;
};
