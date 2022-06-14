#include "parser.h"

std::set<std::string> KernelParser::float_values = {"0.5", "-0.5", "1.0", "-1.0",
                                                    "2.0", "-2.0", "4.0", "-4.0"};
bool KernelParser::is_float(const std::string& arg) {
    // todo 1.0 / (2.0 * PI) unsupported
    return float_values.find(arg) != float_values.end();
}
bool KernelParser::is_scalar(const std::string& arg) {
    if (arg.size() < 2) return false;
    return (arg[0] == 'S' || arg[0] == 's') && !std::isalpha(arg[1]);
}
bool KernelParser::is_vector(const std::string& arg) {
    if (arg.size() < 2) return false;
    return (arg[0] == 'v' || arg[0] == 'V') && !std::isalpha(arg[1]);
}
bool KernelParser::is_label(const std::string& arg) {
    return utils::startsWith(arg, ".L");
}
bool KernelParser::is_hex(const std::string& str) {
    if (utils::startsWith(str, "0x")) {
        return str.size() > 2 && std::all_of(str.begin() + 2, str.end(), ::isxdigit);
    }
    return std::all_of(str.begin(), str.end(), ::isxdigit);
}
bool KernelParser::is_integer(const std::string& str) {
    return !str.empty() &&
           utils::startsWith(str, "-")
           ? std::all_of(str.begin() + 1, str.end(), ::isdigit)
           : std::all_of(str.begin(), str.end(), ::isdigit);
}
std::pair<RegisterType, size_t> KernelParser::get_register(const std::string& arg) {
    if (arg == "vcc") {
        return std::make_pair(VCC, 2);
    }
    if (arg == "scc") {
        return std::make_pair(SCC, 1);
    }
    if (arg == "exec") {
        return std::make_pair(EXEC, 2);
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
            throw std::runtime_error("Unsupported register: " + arg);
        }
        return std::make_pair(registerType, regCount);
    }
    if (is_vector(arg)) {
        auto vgprInd = regCount > 1 ? fromRegInd : stoi(arg.substr(1));
        return std::make_pair(RegisterType(vgprInd + V0), regCount);
    }

    throw std::runtime_error("Unsupported instruction argument: " + arg);
}
Operand KernelParser::parseOperand(const std::string& arg) {
    assert(!arg.empty() && "Instruction argument can not be empty");

    if (is_float(arg)) {
        return Operand{FLOAT_CONST, stof(arg)};
    } else if (is_integer(arg)) {
        return Operand{INT_CONST, stoi(arg)};
    } else if (is_hex(arg)) {
        return Operand{LITERAL_CONST, static_cast<uint32_t>(stoul(arg, nullptr, 16))};
    } else if (is_label(arg)) {
        return Operand{LABEL, static_cast<uint32_t>(stoul(arg.substr(2)))};
    } else {
        auto regData = get_register(arg);
        return Operand{REGISTER, regData.first, regData.second};
    }
}
KernelCode KernelParser::parseKernelCode(const std::vector<std::string>& instructions) {
    KernelCode kernelCode;
    int prevInstrAddr = -1;

    for (auto& instr : instructions) {
        if (is_label(instr)) {
            kernelCode.add_label(static_cast<uint32_t>(stoul(instr.substr(2))), instr);
            continue;
        }
        auto dividedInstr = utils::splitMap<std::string>(instr, ' ',[](auto arg) {
          return utils::trim(arg);
        }, 2);
        assert(dividedInstr.size() >= 2 && "Unsupported asm instruction format");
        auto addressStr = dividedInstr[0].substr(2);
        auto address = stoul(addressStr, nullptr, 16);
        auto mnemonic = dividedInstr[1];
        if (dividedInstr.size() == 3) {
            auto args = utils::splitMap<Operand>(dividedInstr[2], ',', [](auto arg) {
              return parseOperand( utils::trim(arg));
            });
            kernelCode.add_instr(address, mnemonic, mnemonic + " " + dividedInstr[2], args);
        } else {
            kernelCode.add_instr(address, mnemonic, mnemonic);
        }
        if (prevInstrAddr != -1) {
            auto prevInstr = kernelCode.get_instr(prevInstrAddr);
            prevInstr->set_width_and_format(address - prevInstrAddr);
        }
        prevInstrAddr = int(address);
    }
    auto lastInstr = kernelCode.get_instr(prevInstrAddr);
    lastInstr->set_width_and_format(4);
    if (lastInstr->get_key() != S_ENDPGM) {
        logger.error(std::string("Unsupported last instruction: ") +
                     get_mnemonic(lastInstr->get_key()));
    }

    return kernelCode;
}

void validateSizes(size_t dims, const std::vector<size_t>& sizes, const char *name) {
    const char* dimsLetters = "XYZ";
    for (size_t i = 0; i < dims; ++i) {
        if (sizes[i] == 0) {
            throw std::runtime_error(std::string("Invalid ") + name + " for " + dimsLetters[i]);
        }
    }
}

KernelConfig KernelParser::parseKernelConfig(uint32_t dims,
                       const std::vector<size_t>& globalWorkOffset,
                       const std::vector<size_t>& globalWorkSize,
                       const std::vector<size_t>& localWorkSize,
                       const std::vector<std::string>& configParams) {
    KernelConfig config;
    config.dims = dims;

    if (!globalWorkOffset.empty()) {
        std::copy_n(globalWorkOffset.begin(), dims, config.globalWorkOffset.begin());
    }
    if (globalWorkSize.empty()) {
        throw std::runtime_error("Global work size parameter can not be empty!");
    }
    validateSizes(dims, config.globalWorkSize, "Global work size");
    std::copy_n(globalWorkSize.begin(), dims, config.globalWorkSize.begin());
    if (!localWorkSize.empty()) {
        std::copy_n(localWorkSize.begin(), dims, config.localWorkSize.begin());
        validateSizes(dims, config.localWorkSize, "Local work size");
    }
    for (auto &param: configParams) {
        auto dividedParam = utils::split(param.substr(1), ' ', 1);
        const auto &paramName = dividedParam[0];
        if ("sgprsnum" == paramName) {
            config.sgprsnum = std::stoi(dividedParam[1]);
        } else if ("vgprsnum" == paramName) {
            config.vgprsnum = std::stoi(dividedParam[1]);
        } else if ("priority" == paramName) {
            auto priorityValue = std::stoi(dividedParam[1]);
            assert(priorityValue >= 0 && priorityValue <= 3);
            config.priority = (uint8_t) priorityValue;
        } else if ("cws" == paramName) {
            auto workGroupSizes = utils::split(dividedParam[1], ',');
            config.localWorkSize = {std::stoul(workGroupSizes[0]),
                                   workGroupSizes.size() > 1 ? std::stoul(workGroupSizes[1]) : 1,
                                   workGroupSizes.size() > 2 ? std::stoul(workGroupSizes[2]) : 1};
        } else if ("pgmrsrc2" == paramName) {
            config.pgmrsrc2 = std::stoi(dividedParam[1], nullptr, 16);
        } else if ("dx10clamp" == paramName) {
            config.dx10clamp = true;
        } else if ("ieeemode" == paramName) {
            config.ieeemode = true;
        } else if ("useargs" == paramName) {
            config.useargs = true;
        } else if ("usegeneric" == paramName) {
            config.usegeneric = true;
        } else if ("usesetup" == paramName) {
            config.usesetup = true;
        } else {
            logger.warn(std::string("Unsupported configuration parameter: " + param));
        }
    }
    if (config.sgprsnum == 0 || config.vgprsnum == 0) {
        throw std::runtime_error("SGPRs and VGPRs amount should be specified");
    }

    return config;
}
