#ifndef RED_O_LATOR_ICD_H
#define RED_O_LATOR_ICD_H

#include "IcdDispatchTable.h"
#include "rename-api.h"

#include <opencl.h>

struct CLPlatformId {
    IcdDispatchTable* dispatchTable;
    const char* profile;
    const char* version;
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

extern IcdDispatchTable* mDispatchTable;

#endif  // RED_O_LATOR_ICD_H
