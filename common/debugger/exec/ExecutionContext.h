#ifndef RED_O_LATOR_EXECUTIONCONTEXT_H
#define RED_O_LATOR_EXECUTIONCONTEXT_H

#include <cstdint>
#include "ExecutionInfo.h"

struct ExecutionContext {
    int wavefrontId = 0;
    int workItemX = 0;
    int workItemY = 0;
    int workItemZ = 0;

    uint64_t address = 0;

    ExecutionInfo info;
};

#endif  // RED_O_LATOR_EXECUTIONCONTEXT_H
