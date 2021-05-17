#pragma once

#include "icd.h"

struct CLDeviceId {
    CLDeviceId(IcdDispatchTable* const dispatchTable,
               cl_device_type deviceType,
               size_t globalMemorySize,
               size_t constantMemorySize,
               size_t localMemorySize)
        : dispatchTable(dispatchTable),
          deviceType(deviceType),
          globalMemorySize(globalMemorySize),
          constantMemorySize(constantMemorySize),
          localMemorySize(localMemorySize) {}

    IcdDispatchTable* const dispatchTable;
    cl_device_type deviceType;

    size_t globalMemorySize = 0;
    size_t constantMemorySize = 0;
    size_t localMemorySize = 0;

    size_t usedGlobalMemory = 0;
    size_t usedLocalMemory = 0;

    [[nodiscard]] bool matchesType(cl_device_type type) const {
        return type == CL_DEVICE_TYPE_ALL || (type ^ deviceType) == 0;
    }
};
