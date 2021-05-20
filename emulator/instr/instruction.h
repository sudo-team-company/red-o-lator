//
// Created by Diana Kudaiberdieva
//

#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include "../../common/common/common.hpp"
#include "../reg/reg_info.h"
#include "instr_info.h"

enum OperandType { REGISTER, FLOAT, INT_CONST, LITERAL_CONST };

struct Operand {
    OperandType type;
    std::variant<int, uint32_t, float, RegisterType> value;
    int lgkmcnt = -1;
    int vmcnt = -1;
    size_t regAmount = 0;

    explicit Operand(const std::string&);

    OperandType get_type() const {
        return type;
    }

    size_t get_reg_amount() const {
        return regAmount;
    }

   private:
    static std::set<std::string> float_values;
    bool is_float(const std::string&);
    bool is_scalar(const std::string&);
    bool is_vector(const std::string&);
    std::pair<RegisterType, size_t> get_register(const std::string&);
};

struct Instruction {
    InstrKey get_key() const {
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

    Instruction(const std::string& addr,
                const std::string& instr,
                const std::vector<std::string>& args);
   private:
    InstrKey instrKey;
    std::vector<std::unique_ptr<Operand>> operands;
    uint32_t addr;
};

struct KernelCode {
    Instruction* get_instr(uint64_t);

   private:
    std::unordered_map<std::uint64_t, std::unique_ptr<Instruction>> code;
};
