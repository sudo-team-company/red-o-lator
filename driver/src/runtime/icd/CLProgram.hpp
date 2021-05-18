#pragma once

#include "runtime/program/BinaryDisassembler.h"
#include "icd.h"

struct CLProgram {
    explicit CLProgram(IcdDispatchTable* dispatchTable, CLContext* context)
        : dispatchTable(dispatchTable), context(context) {}

    IcdDispatchTable* const dispatchTable;
    CLContext* context;

    size_t binarySize;
    const unsigned char* binary;

    std::unique_ptr<BinaryDisassemblingResult> disassembledBinary;

    unsigned int referenceCount = 1;
};
