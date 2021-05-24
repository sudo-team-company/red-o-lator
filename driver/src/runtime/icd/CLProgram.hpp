#pragma once

#include "runtime/program/BinaryDisassembler.h"
#include "icd.h"

struct CLProgram {
    explicit CLProgram(IcdDispatchTable* dispatchTable, CLContext* context)
        : dispatchTable(dispatchTable), context(context) {
        clRetainContext(context);
    }

    ~CLProgram() {
        for (auto kernel : disassembledBinary->kernels) {
            clReleaseKernel(kernel);
        }
        delete[] binary;
        clReleaseContext(context);
    }

    IcdDispatchTable* const dispatchTable;
    CLContext* context;

    size_t binarySize = 0;
    const std::byte* binary = nullptr;

    cl_build_status buildStatus = CL_BUILD_NONE;
    std::string buildLog;
    std::unique_ptr<BinaryDisassemblingResult> disassembledBinary;

    unsigned int referenceCount = 1;
};
