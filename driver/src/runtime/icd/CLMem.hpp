#pragma once

#include <cstddef>

#include "icd.h"

struct CLMem {
    explicit CLMem(IcdDispatchTable* dispatchTable, CLContext* context)
        : dispatchTable(dispatchTable), context(context) {}

    IcdDispatchTable* const dispatchTable;
    CLContext const* context;

    std::byte* address = nullptr;
    size_t size = 0;

    bool kernelCanRead = false;
    bool kernelCanWrite = false;

    bool hostCanRead = false;
    bool hostCanWrite = false;

    unsigned int referenceCount = 1;
};
