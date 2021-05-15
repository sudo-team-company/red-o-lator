//
// Created by Diana Kudaiberdieva
//

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "../reg/reg_info.h"
#include "instr_info.h"

using namespace reg;

enum OperandType { REGISTER, FLOAT, INT_CONST, LITERAL_CONST };

struct Operand {
    OperandType type;
    std::variant<int16_t, uint32_t, double, RegisterType> value;
    int waitcnt;
    size_t reg_amount = 0;

    explicit Operand(int16_t i) : type(INT_CONST), value(i) {}
    explicit Operand(uint32_t u) : type(LITERAL_CONST), value(u) {}
    explicit Operand(double d) : type(FLOAT), value(d) {}
    explicit Operand(RegisterType regs, size_t n)
        : type(REGISTER), value(regs), reg_amount(n) {}

    OperandType get_type() const {
        return type;
    }

    size_t get_reg_amount() const {
        return reg_amount;
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

   private:
    InstrKey instrKey;
    std::vector<std::unique_ptr<Operand>> operands;
};
