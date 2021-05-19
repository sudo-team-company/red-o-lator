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

std::set<std::string> Operand::float_values = {"0.5", "-0.5", "1.0", "-1.0",
                                               "2.0", "-2.0", "4.0", "-4.0"};

Operand::Operand(const std::string& arg) {
    if (is_float(arg)) {
        type = FLOAT;
        value = std::stof(arg);
    } else if (utils::is_integer(arg)) {
        type = INT_CONST;
        value = stoi(arg);
    } else if (utils::is_hex(arg)) {
        type = LITERAL_CONST;
        value = uint32_t(stoul(arg, nullptr, 16));
    } else {
        type = REGISTER;
        auto regData = get_register(arg);
        value = regData.first;
        regAmount = regData.second;
    }
}

bool Operand::is_float(const std::string& arg) {
    // todo 1.0 / (2.0 * PI) unsupported
    return float_values.find(arg) != float_values.end();
}

bool Operand::is_scalar(const std::string& arg) {
    if (arg.size() < 2) return false;
    return (arg[0] == 'S' || arg[0] == 's') && !std::isalpha(arg[1]);
}

bool Operand::is_vector(const std::string& arg) {
    if (arg.size() < 2) return false;
    return (arg[0] == 'v' || arg[0] == 'V') && !std::isalpha(arg[1]);
}

std::pair<RegisterType, size_t> Operand::get_register(const std::string& arg) {
    if (arg == "vcc") {
        return std::make_pair(VCC, 0);
    }
    if (arg == "scc") {
        return std::make_pair(SCC, 0);
    }

    if (arg.find('[') != -1) {
        // todo
    }
    if (is_scalar(arg)) {
        auto sgprInd = stoi(arg.substr(1));
        return std::make_pair(RegisterType(sgprInd + S0), 0);
    }
    if (is_vector(arg)) {
        auto vgprInd = stoi(arg.substr(1));
        return std::make_pair(RegisterType(vgprInd + V0), 0);
    }

    throw std::runtime_error("Unsupported instruction argument: " + arg);
}