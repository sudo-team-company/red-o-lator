#ifndef RED_O_LATOR_PARSER_H
#define RED_O_LATOR_PARSER_H

#include <string>
#include <vector>
#include "commons.h"
#include "model/instr/instruction.h"
#include "model/kernel/KernelConfig.h"

class KernelParser {

    static std::set<std::string> float_values;

    static bool is_float(const std::string& arg);
    static bool is_scalar(const std::string& arg);
    static bool is_vector(const std::string& arg);
    static bool is_label(const std::string& arg);
    static bool is_hex(const std::string &str);
    static bool is_integer(const std::string &str);

    static std::pair<RegisterType, size_t> get_register(const std::string& arg);

    static Operand parseOperand(const std::string& arg);

   public:
    static KernelCode parseKernelCode(const std::vector<std::string>& instructions);
    static KernelConfig parseKernelConfig(uint32_t dims,
                      const std::vector<size_t>& globalWorkOffset,
                      const std::vector<size_t>& globalWorkSize,
                      const std::vector<size_t>& localWorkSize,
                      const std::vector<std::string>& configParams);
};

#endif  // RED_O_LATOR_PARSER_H
