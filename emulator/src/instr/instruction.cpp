#include "instruction.h"

KernelCode::KernelCode(const std::vector<std::string> &instruction) {
    int prevInstrAddr = -1;
    for (auto &instr: instruction) {
        if (Operand::is_label(instr)) {
            continue;
        }
        auto dividedInstr = utils::split(instr, ' ', 2);
        assert(dividedInstr.size() >= 2 && "Unsupported asm instruction format");
        auto addressStr = utils::trim(dividedInstr[0].substr(2));
        auto address = stoul(addressStr, nullptr, 16);
        auto mnemonic = utils::trim(dividedInstr[1]);
        if (dividedInstr.size() == 3) {
            auto args = utils::split(dividedInstr[2], ',');
            std::for_each(args.begin(), args.end(), utils::trimInplace);
            add_instr(address, mnemonic, args);
        } else {
            add_instr(address, mnemonic);
        }
        if (prevInstrAddr != -1) {
            auto prevInstr = get_instr(prevInstrAddr);
            prevInstr->set_width_and_format(address - prevInstrAddr);
        }
        prevInstrAddr = int(address);
    }
    auto lastInstr = get_instr(prevInstrAddr);
    lastInstr->set_width_and_format(4);
    if (lastInstr->get_key() != S_ENDPGM) {
        logger.error(std::string("Unsupported last instruction: ") +
                     get_mnemonic(lastInstr->get_key()));
    }
}

Instruction *KernelCode::get_instr(uint64_t address) const {
    assert(address % 4 == 0 && "Wrong instr address: not a multiple of 4");
    auto instrIter = code.find(address);
    assert(instrIter != code.end() && "Wrong instruction address: instr is nullptr");
    return instrIter->second.get();
}

void KernelCode::add_instr(uint32_t addr,
                           std::string &instr,
                           const std::vector<std::string> &args) {
    code[addr] = std::make_unique<Instruction>(Instruction{addr, instr, args});
}

std::set<std::string> Operand::float_values = {"0.5", "-0.5", "1.0", "-1.0",
                                               "2.0", "-2.0", "4.0", "-4.0"};

Operand::Operand(const std::string &arg) {
    if (is_float(arg)) {
        type = FLOAT;
        value = stof(arg);
    } else if (is_integer(arg)) {
        type = INT_CONST;
        value = stoi(arg);
    } else if (is_hex(arg)) {
        type = LITERAL_CONST;
        value = static_cast<uint32_t>(stoul(arg, nullptr, 16));
    } else if (is_label(arg)) {
        type = LABEL;
        auto address = static_cast<uint32_t>(stoul(arg.substr(2)));
        value = address;
    } else {
        type = REGISTER;
        auto regData = get_register(arg);
        value = regData.first;
        regAmount = regData.second;
    }
}

bool Operand::is_float(const std::string &arg) {
    // todo 1.0 / (2.0 * PI) unsupported
    return float_values.find(arg) != float_values.end();
}

bool Operand::is_scalar(const std::string &arg) {
    if (arg.size() < 2) return false;
    return (arg[0] == 'S' || arg[0] == 's') && !std::isalpha(arg[1]);
}

bool Operand::is_vector(const std::string &arg) {
    if (arg.size() < 2) return false;
    return (arg[0] == 'v' || arg[0] == 'V') && !std::isalpha(arg[1]);
}

bool Operand::is_label(const std::string &arg) {
    return utils::startsWith(arg, ".L");
}

std::pair<RegisterType, size_t> Operand::get_register(const std::string &arg) {
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
        auto registerType = RegisterType(sgprInd + S0);
        if (registerType > S103) {
            logger.error(std::string("Unsupported scalar register: " + arg));
            throw std::runtime_error("Unsupported register!");
        }
        return std::make_pair(registerType, regCount);
    }
    if (is_vector(arg)) {
        auto vgprInd = regCount > 1 ? fromRegInd : stoi(arg.substr(1));
        return std::make_pair(RegisterType(vgprInd + V0), regCount);
    }

    throw std::runtime_error("Unsupported instruction argument: " + arg);
}

Instruction::Instruction(uint32_t addr,
                         std::string &instr,
                         const std::vector<std::string> &args)
                         : addr(addr), rawInstruction(std::move(instr)) {
    instrKey = get_instr_key(rawInstruction);
    operands = std::vector<std::unique_ptr<Operand>>();
    for (auto &arg: args) {
        assert(!args.empty() && "Instruction argument can not be empty");
        operands.push_back(std::make_unique<Operand>(arg));
    }
}
