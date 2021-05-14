#pragma once

#include "icd.h"

struct CLSampler {
    explicit CLSampler(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};
