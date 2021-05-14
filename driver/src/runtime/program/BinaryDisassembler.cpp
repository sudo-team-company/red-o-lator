#include <CLRX/amdasm/Disassembler.h>
#include <common/common.hpp>
#include <iostream>
#include <sstream>

#include "BinaryDisassembler.h"

std::unique_ptr<BinaryDisassemblingResult> BinaryDisassembler::disassemble(
    size_t binarySize, const unsigned char* binary) const {
    const auto amdInput = CLRX::AmdCL2MainGPUBinary64(
        binarySize, const_cast<unsigned char*>(binary));
    std::ostringstream disasmOss;

    //    CLRX::DISASM_ALL & ~(CLRX::DISASM_CODEPOS | CLRX::DISASM_HEXCODE);
    //    CLRX::Flags disasmFlags = CLRX::FLAGS_ALL;
    CLRX::Flags disasmFlags =
        CLRX::DISASM_ALL & ~(CLRX::DISASM_HEXCODE) | CLRX::DISASM_CONFIG;
    CLRX::Disassembler disasm(amdInput, disasmOss, disasmFlags);
    disasm.disassemble();

    std::unique_ptr<BinaryDisassemblingResult> parsingResult =
        BinaryAsmParser::parseAsm(disasmOss.str());

    std::cout << disasmOss.str() << std::endl;

    return parsingResult;
}
