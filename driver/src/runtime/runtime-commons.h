#pragma once

#include <common/Logger.h>
#include "runtime/device/DeviceConfigurationParser.h"

extern Logger kLogger;
extern IcdDispatchTable* kDispatchTable;
extern DeviceConfigurationParser kDeviceConfigurationParser;
extern CLPlatformId* kPlatform;
extern CLDeviceId* kDevice;

#define RETURN_ERROR(errorCode, message)                              \
    do {                                                              \
        kLogger.error(std::string(message) + " Code: " + #errorCode); \
        return errorCode;                                             \
    } while (0);

#define SET_ERROR_AND_RETURN(errorCode, message)                                 \
    do {                                                              \
        kLogger.error(std::string(message) + " Code: " + #errorCode); \
        if (errcode_ret) {                                            \
            *errcode_ret = errorCode;                                 \
        }                                                             \
        return nullptr;                                               \
    } while (0);

#define SET_SUCCESS()                \
    if (errcode_ret) {             \
        *errcode_ret = CL_SUCCESS; \
    }

namespace utils {
extern bool isMutuallyExclusive(cl_bitfield flags,
                                std::initializer_list<cl_int> checkFlags);
}  // namespace utils
