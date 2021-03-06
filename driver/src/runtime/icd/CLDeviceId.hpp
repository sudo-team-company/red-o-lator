#pragma once

#include "icd.h"

// TODO: encapsulate parser in device

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
    const cl_device_type deviceType;

    const size_t globalMemorySize;
    const size_t constantMemorySize;
    const size_t localMemorySize;

    size_t usedGlobalMemory = 0;
    size_t usedLocalMemory = 0;

    [[nodiscard]] bool matchesType(cl_device_type type) const {
        return (type & deviceType) != 0;
    }
};
