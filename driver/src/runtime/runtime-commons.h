#pragma once

#include <common/Logger.h>
#include <cstring>
#include <functional>
#include <optional>
#include <variant>

#include "CLObjectInfoParameterValue.hpp"
#include "device/DeviceConfigurationParser.h"

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

#define SET_ERROR_AND_RETURN(errorCode, message)                      \
    do {                                                              \
        kLogger.error(std::string(message) + " Code: " + #errorCode); \
        if (errcode_ret) {                                            \
            *errcode_ret = errorCode;                                 \
        }                                                             \
        return nullptr;                                               \
    } while (0);

#define SET_SUCCESS()                  \
    do {                               \
        if (errcode_ret) {             \
            *errcode_ret = CL_SUCCESS; \
        }                              \
    } while (0);

extern cl_int getParamInfo(
    cl_uint param_name,
    size_t param_value_size,
    void* param_value,
    size_t* param_value_size_ret,
    const std::function<std::optional<CLObjectInfoParameterValue>()>&
        parameterValueGetter);

namespace utils {
extern bool isMutuallyExclusive(cl_bitfield flags,
                                std::initializer_list<cl_int> checkFlags);
}  // namespace utils
