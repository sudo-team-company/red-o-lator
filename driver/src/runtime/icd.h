#pragma once

#ifndef CL_TARGET_OPENCL_VERSION
#define CL_TARGET_OPENCL_VERSION 120
#endif

/** It is important that rename-api.h is included before <opencl.h> to rename
 *  OpenCL API entries to avoid clash with icd functions.
 *
 *  For that reason, this file should be imported instead of <opencl.h> in
 *  outer files.
 */
#include "rename-api.h"

#include <CL/opencl.h>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "IcdDispatchTable.h"

using CLContextCallback = void(CL_CALLBACK*)(const char* errinfo,
                                             const void* private_info,
                                             size_t cb,
                                             void* user_data);

struct CLPlatformId {
    explicit CLPlatformId(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
    std::string profile;
    std::string openClVersion;
    std::string driverVersion;
    std::string name;
    std::string vendor;
    std::string extensions;
    std::string suffix;
};

struct CLDeviceId {
    CLDeviceId(IcdDispatchTable* const dispatchTable,
               size_t globalMemorySize,
               size_t constantMemorySize,
               size_t localMemorySize)
        : dispatchTable(dispatchTable),
          globalMemorySize(globalMemorySize),
          constantMemorySize(constantMemorySize),
          localMemorySize(localMemorySize) {}

    IcdDispatchTable* const dispatchTable;
    size_t globalMemorySize;
    size_t constantMemorySize;
    size_t localMemorySize;
};

struct CLContext {
    CLContext(IcdDispatchTable* dispatchTable, CLDeviceId* device)
        : dispatchTable(dispatchTable), device(device) {}

    IcdDispatchTable* const dispatchTable;
    CLDeviceId* device;

    std::optional<CLContextCallback> callback = std::nullopt;
    void* callbackUserData = nullptr;
    unsigned int referenceCount = 0;
};

struct Command {};

struct CLCommandQueue {
    explicit CLCommandQueue(IcdDispatchTable* dispatchTable)
        : dispatchTable(dispatchTable) {}

    IcdDispatchTable* const dispatchTable;
    std::vector<Command> commands = std::vector<Command>();
    unsigned int referenceCount = 0;
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
    unsigned int referenceCount = 0;
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
