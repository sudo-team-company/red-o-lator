#pragma once

#include <common/utils/common.hpp>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include "commons/commons.h"
#include "commons/util.h"
#include "instr_info.h"
#include "reg_info.h"

enum OperandType { REGISTER, FLOAT, INT_CONST, LITERAL_CONST, LABEL };

struct Operand {
    OperandType type;
    std::variant<int, uint32_t, float, RegisterType> value;
    size_t regAmount = 0;

    explicit Operand(const std::string&);

    static bool is_label(const std::string&);
   private:
    static std::set<std::string> float_values;
    static bool is_float(const std::string&);
    static bool is_scalar(const std::string&);
    static bool is_vector(const std::string&);
    static std::pair<RegisterType, size_t> get_register(const std::string&);
};

struct Instruction {
    Instruction(uint32_t addr,
                std::string instr,
                const std::vector<std::string>& args);

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

    void set_width_and_format(size_t w) {
        widthInBytes = w;
        format = get_instr_format(instrKey, widthInBytes);
    }

    size_t get_width() const {
        if (widthInBytes != 4 && widthInBytes != 8) {
            logger.error(std::string("Wrong instruction widthInBytes ") +
                         std::to_string(widthInBytes) + " for " + get_mnemonic(instrKey));
            assert(widthInBytes == 4 || widthInBytes == 8);
        }
        return widthInBytes;
    }

    const std::string& get_raw_instr() const { return rawInstruction; }

    InstrFormat get_format() const {
        return format;
    }

   private:
    InstrKey instrKey;
    InstrFormat format = UNDEFINED;
    size_t widthInBytes = 0;
    uint32_t addr;
    std::string rawInstruction;
    std::vector<std::unique_ptr<Operand>> operands;
};

struct KernelCode {
    explicit KernelCode(const std::vector<std::string>&);
    Instruction* get_instr(uint64_t) const;
    void add_instr(uint32_t addr,
                   std::string& instr,
                   const std::vector<std::string>& args = {});
   private:
    std::unordered_map<std::uint64_t, std::unique_ptr<Instruction>> code = std::unordered_map<std::uint64_t, std::unique_ptr<Instruction>>();
};
