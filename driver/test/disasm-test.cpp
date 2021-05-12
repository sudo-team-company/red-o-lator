#include <CLRX/amdasm/Disassembler.h>
#include <iostream>
#include <sstream>

using namespace CLRX;

int main() {
    std::cout << "driver-disasm-test works!";
}

void test(AmdDisasmInput* amdInput) {
    std::ostringstream disasmOss;
    std::string resultStr;
    CLRX::Flags disasmFlags = CLRX::DISASM_ALL&~CLRX::DISASM_CODEPOS;

    CLRX::Disassembler disasm(amdInput, disasmOss, disasmFlags);
    disasm.disassemble();
    resultStr = disasmOss.str();

    std::cout << resultStr;
}
