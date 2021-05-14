#pragma once

#include <memory>
#include <string>
#include "runtime/icd/CLKernel.h"

struct BinaryDisassemblingResult {
    std::string gpuName;
    std::string compileOptions;
    std::string otherParameters;
    std::vector<CLKernel> kernels = std::vector<CLKernel>();
};

class BinaryAsmParser {
   public:
    static std::unique_ptr<BinaryDisassemblingResult> parseAsm(
        const std::string& input);
};
