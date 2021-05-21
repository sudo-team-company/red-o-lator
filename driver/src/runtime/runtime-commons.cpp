#include "runtime-commons.h"

Logger kLogger = Logger("[red-o-lator driver]");  // NOLINT(cert-err58-cpp)

IcdDispatchTable* kDispatchTable =  // NOLINT(cert-err58-cpp)
    IcdDispatchTableProvider().get();

DeviceConfigurationParser kDeviceConfigurationParser =
    DeviceConfigurationParser();

CLPlatformId* kPlatform = nullptr;
CLDeviceId* kDevice = nullptr;

cl_int getParamInfo(
    cl_uint param_name,
    size_t param_value_size,
    void* param_value,
    size_t* param_value_size_ret,
    const std::function<std::optional<CLObjectInfoParameterValue>()>&
        parameterValueGetter) {
    if (!param_value && !param_value_size_ret) {
        return CL_SUCCESS;
    }

    auto parameterValue = parameterValueGetter();

    if (!parameterValue.has_value()) {
        RETURN_ERROR(CL_INVALID_VALUE, "Unknown device info parameter " +
                                           std::to_string(param_name) + ".")
    }

    auto [result, resultSize] = parameterValue.value();

    if (std::holds_alternative<std::string>(result)) {
        resultSize = strlen(std::get<std::string>(result).c_str()) + 1;
    }

    if (param_value_size && param_value_size < resultSize) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "Not enough size to fit parameter: need " +
                         std::to_string(resultSize) + ", got " +
                         std::to_string(param_value_size) + ".")
    }

    if (param_value) {
        if (std::holds_alternative<void*>(result)) {
            memcpy(param_value, &std::get<void*>(result), resultSize);

        } else if (std::holds_alternative<CLObjectInfoParameterValueTypeArray>(
                       result)) {
            memcpy(param_value,
                   std::get<CLObjectInfoParameterValueTypeArray>(result).array,
                   resultSize);
        } else {
            memcpy(param_value, std::get<std::string>(result).c_str(),
                   resultSize);
        }
    }

    if (param_value_size_ret) {
        *param_value_size_ret = resultSize;
    }

    return CL_SUCCESS;
}

bool utils::hasMutuallyExclusiveFlags(
    cl_bitfield flags, std::initializer_list<cl_int> checkFlags) {
    bool foundFlag = false;

    for (auto flag : checkFlags) {
        const bool flagIsSet = (flags & flag);
        if (flagIsSet && !foundFlag) {
            foundFlag = true;
        } else if (flagIsSet) {
            return true;
        }
    }

    return false;
}
