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

#define SET_ERROR_AND_RETURN(errorCode, message)                      \
    do {                                                              \
        kLogger.error(std::string(message) + " Code: " + #errorCode); \
        if (errcode_ret) {                                            \
            *errcode_ret = errorCode;                                 \
        }                                                             \
        return nullptr;                                               \
    } while (0);

#define SET_SUCCESS()              \
    if (errcode_ret) {             \
        *errcode_ret = CL_SUCCESS; \
    }

#define GET_PARAM_INFO(fn)                                                    \
    if (!param_value && !param_value_size_ret) {                              \
        return CL_SUCCESS;                                                    \
    }                                                                         \
                                                                              \
    std::variant<void*, std::string> result;                                  \
    size_t resultSize;                                                        \
                                                                              \
    const auto error = fn();                                                  \
                                                                              \
    if (error) {                                                              \
        return error;                                                         \
    }                                                                         \
                                                                              \
    if (std::holds_alternative<std::string>(result)) {                        \
        resultSize = std::get<std::string>(result).size() + 1;                \
    }                                                                         \
                                                                              \
    if (param_value_size && param_value_size < resultSize) {                  \
        RETURN_ERROR(CL_INVALID_VALUE, "Not enough size to fit parameter: " + \
                                           std::to_string(param_name))        \
    }                                                                         \
                                                                              \
    if (param_value) {                                                        \
        if (std::holds_alternative<void*>(result)) {                          \
            memcpy(param_value, &std::get<void*>(result), resultSize);        \
        } else {                                                              \
            memcpy(param_value, std::get<std::string>(result).c_str(),        \
                   resultSize);                                               \
        }                                                                     \
    }                                                                         \
                                                                              \
    if (param_value_size_ret) {                                               \
        *param_value_size_ret = resultSize;                                   \
    }                                                                         \
                                                                              \
    return CL_SUCCESS;

namespace utils {
extern bool isMutuallyExclusive(cl_bitfield flags,
                                std::initializer_list<cl_int> checkFlags);
}  // namespace utils
