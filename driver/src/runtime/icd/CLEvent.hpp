#pragma once

#include "icd.h"

struct CLEvent {
    explicit CLEvent(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};
