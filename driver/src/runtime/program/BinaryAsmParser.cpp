#include <common/common.hpp>

#include "BinaryAsmParser.h"

std::unique_ptr<BinaryDisassemblingResult> BinaryAsmParser::parseAsm(
    const std::string& input) {
    auto lines = utils::split(input, '\n');


    const auto parsingResult = BinaryDisassemblingResult();

    for (auto& line : lines) {
        utils::trimInplace(line);
        utils::split(line, ' ', 1);
    }

    return std::unique_ptr<BinaryDisassemblingResult>();
}
