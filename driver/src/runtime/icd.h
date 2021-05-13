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

#include "IcdDispatchTable.h"

struct CLPlatformId {
    IcdDispatchTable* dispatchTable;
    const char* profile;
    const char* openClVersion;
    const char* driverVersion;
    const char* name;
    const char* vendor;
    const char* extensions;
    const char* suffix;
};

struct CLDeviceId {
    IcdDispatchTable* dispatchTable;
};

struct CLContext {
    IcdDispatchTable* dispatchTable;
};

struct CLCommandQueue {
    IcdDispatchTable* dispatchTable;
};

struct CLMem {
    IcdDispatchTable* dispatchTable;
};

struct CLProgram {
    IcdDispatchTable* dispatchTable;
};

struct CLKernel {
    IcdDispatchTable* dispatchTable;
};

struct CLEvent {
    IcdDispatchTable* dispatchTable;
};

struct CLSampler {
    IcdDispatchTable* dispatchTable;
};
