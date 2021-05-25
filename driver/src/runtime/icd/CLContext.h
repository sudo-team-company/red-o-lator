#pragma once

#include <optional>

#include "icd.h"

using CLContextCallback = void(CL_CALLBACK*)(const char* errinfo,
                                             const void* private_info,
                                             size_t cb,
                                             void* user_data);

struct CLContext {
    CLContext(IcdDispatchTable* dispatchTable, CLDeviceId* device)
        : dispatchTable(dispatchTable), device(device) {}

    IcdDispatchTable* const dispatchTable;
    CLDeviceId* const device;

    std::optional<CLContextCallback> callback = std::nullopt;
    void* callbackUserData = nullptr;
    unsigned int referenceCount = 1;
};
