#include "instruction.h"

Instruction* KernelCode::get_instr(uint64_t address) {
    assert(address % 4 == 0&& "Wrong instr address: not a multiple of 4");
    Instruction* instr = code[address].get();
    assert(instr != nullptr && "Wrong instruction address: instr is nullptr");
    return instr;
}
void KernelCode::add_instr(uint32_t addr,
                           const std::string& instr,
                           const std::vector<std::string>& args) {
    code[addr] = std::make_unique<Instruction>(Instruction{addr, instr, args});
}

std::set<std::string> Operand::float_values = {"0.5", "-0.5", "1.0", "-1.0",
                                               "2.0", "-2.0", "4.0", "-4.0"};

Operand::Operand(const std::string& arg) {
    if (is_float(arg)) {
        type = FLOAT;
        value = std::stof(arg);
    } else if (is_integer(arg)) {
        type = INT_CONST;
        value = stoi(arg);
    } else if (is_hex(arg)) {
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
        return std::make_pair(VCC, 1);
    }
    if (arg == "scc") {
        return std::make_pair(SCC, 1);
    }
    if (utils::startsWith(arg, "lgkmcnt")) {
        return std::make_pair(LGKMCNT, 1);
    }
    if (utils::startsWith(arg, "vmcnt")) {
        return std::make_pair(VMCNT, 1);
    }
    auto bracketPos = arg.find('[');
    size_t regCount = 1;
    int fromRegInd;
    if (bracketPos != -1) {
        auto colonPos = arg.find(':');
        if (colonPos == -1) {
            throw std::runtime_error("Invalid instruction argument: " + arg);
        }
        fromRegInd = stoi(arg.substr(bracketPos + 1));
        auto toRegInd = stoi(arg.substr(colonPos + 1));
        regCount += toRegInd - fromRegInd;
        if (regCount <= 0) throw std::runtime_error("Invalid register range: " + arg);
    }

    if (is_scalar(arg)) {
        auto sgprInd = regCount > 1 ? fromRegInd : stoi(arg.substr(1));
        return std::make_pair(RegisterType(sgprInd + S0), regCount);
    }
    if (is_vector(arg)) {
        auto vgprInd = regCount > 1 ? fromRegInd : stoi(arg.substr(1));
        return std::make_pair(RegisterType(vgprInd + V0), regCount);
    }

    throw std::runtime_error("Unsupported instruction argument: " + arg);
}
Instruction::Instruction(uint32_t addr,
                         const std::string& instr,
                         const std::vector<std::string>& args): addr(addr) {
    instrKey = get_instr_key(instr);
    operands = std::vector<std::unique_ptr<Operand>>();
    for (auto& arg : args) {
        if (arg.empty()) {
            // todo log strange behaviour
        }
        operands.push_back(std::make_unique<Operand>(arg));
    }
}
