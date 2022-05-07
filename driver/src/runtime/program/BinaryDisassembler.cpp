#include <CLRX/amdasm/Disassembler.h>
#include <sstream>

#include "BinaryDisassembler.h"

std::unique_ptr<BinaryDisassemblingResult> BinaryDisassembler::disassemble(
    size_t binarySize, const std::byte* binary) const {
    std::unique_ptr<CLRX::AmdMainBinaryBase> base(CLRX::createAmdCL2BinaryFromCode(binarySize, (unsigned char*) binary));
    CLRX::AmdCL2MainGPUBinary64* amdInput =
        static_cast<CLRX::AmdCL2MainGPUBinary64*>(base.get());
    std::ostringstream disasmOss;

    //    CLRX::DISASM_ALL & ~(CLRX::DISASM_CODEPOS | CLRX::DISASM_HEXCODE);
    //    CLRX::Flags disasmFlags = CLRX::FLAGS_ALL;
    CLRX::Flags disasmFlags =
        CLRX::DISASM_ALL & ~(CLRX::DISASM_HEXCODE) | CLRX::DISASM_CONFIG;
    CLRX::Disassembler disasm(*amdInput, disasmOss, disasmFlags);
    disasm.disassemble();

    const std::string input = disasmOss.str();
    auto parser = BinaryAsmParser(std::make_shared<std::string>(input));

    std::unique_ptr<BinaryDisassemblingResult> parsingResult =
        parser.parseAsm();

    parsingResult->rawOutput = input;

    return parsingResult;
}
