#pragma once

/** It is important that rename-api.h is included before <opencl.h> to rename
 *  OpenCL API entries to avoid clash with icd functions.
 *
 *  For that reason, this file should be imported instead of <opencl.h> in
 *  outer files.
 */
#include "rename-api.h"

#include <opencl.h>
#include <string>
#include <optional>

#include "IcdDispatchTable.h"

using CLContextCallback = void(CL_CALLBACK*)(const char* errinfo,
                                             const void* private_info,
                                             size_t cb,
                                             void* user_data);

struct CLPlatformId {
    explicit CLPlatformId(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
    const char* profile = "";
    const char* openClVersion = "";
    const char* driverVersion = "";
    const char* name = "";
    const char* vendor = "";
    const char* extensions = "";
    const char* suffix = "";
};

struct CLDeviceId {
    explicit CLDeviceId(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};

struct CLContext {
    explicit CLContext(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
    std::optional<CLContextCallback> callback = std::nullopt;
    void* callbackUserData = nullptr;
    unsigned int referenceCount = 0;
};

struct CLCommandQueue {
    explicit CLCommandQueue(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};

struct CLMem {
    explicit CLMem(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};

struct CLProgram {
    explicit CLProgram(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}
    IcdDispatchTable* const dispatchTable;
};

struct CLKernel {
    explicit CLKernel(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};

struct CLEvent {
    explicit CLEvent(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};

struct CLSampler {
    explicit CLSampler(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
};
