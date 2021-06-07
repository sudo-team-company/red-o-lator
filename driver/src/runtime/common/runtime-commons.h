#pragma once

#include <cstring>
#include <functional>
#include <memory>
#include <optional>
#include <variant>

#include "common/utils/time-utils.hpp"

#include "runtime/CLObjectInfoParameterValue.hpp"
#include "runtime/device/DeviceConfigurationParser.h"
#include "runtime/icd/CLPlatformId.hpp"
#include "runtime/icd/CLCommandQueue.h"
#include "runtime/icd/IcdDispatchTable.h"
#include "runtime/common/logger.h"

extern IcdDispatchTable* kDispatchTable;
extern DeviceConfigurationParser kDeviceConfigurationParser;
extern utils::Clock* kClock;

extern CLPlatformId* kPlatform;
extern CLDeviceId* kDevice;

#define RETURN_ERROR(errorCode, message)                              \
    do {                                                              \
        kLogger.error(std::string(message) + " Code: " + #errorCode); \
        return errorCode;                                             \
    } while (0)

#define SET_ERROR_AND_RETURN(errorCode, message)                      \
    do {                                                              \
        kLogger.error(std::string(message) + " Code: " + #errorCode); \
        if (errcode_ret) {                                            \
            *errcode_ret = errorCode;                                 \
        }                                                             \
        return nullptr;                                               \
    } while (0)

#define SET_SUCCESS()                  \
    do {                               \
        if (errcode_ret) {             \
            *errcode_ret = CL_SUCCESS; \
        }                              \
    } while (0)

extern cl_int getParamInfo(
    cl_uint param_name,
    size_t param_value_size,
    void* param_value,
    size_t* param_value_size_ret,
    const std::function<std::optional<CLObjectInfoParameterValue>()>&
        parameterValueGetter);

extern void enqueueCommand(cl_command_queue queue,
                      cl_uint waitListEventsCount,
                      const cl_event* waitList,
                      cl_event* eventOut,
                      const std::function<Command*()>& commandGetter);

extern void registerCall(const std::string& funcName);

namespace utils {
extern bool hasMutuallyExclusiveFlags(cl_bitfield flags,
                                      std::initializer_list<cl_int> checkFlags);
}  // namespace utils
