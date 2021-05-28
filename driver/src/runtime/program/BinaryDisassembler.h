#pragma once

#include <cstddef>
#include <memory>

#include "BinaryAsmParser.h"

class BinaryDisassembler {
   public:
    std::unique_ptr<BinaryDisassemblingResult> disassemble(
        size_t binarySize, const std::byte* binary) const;
};
