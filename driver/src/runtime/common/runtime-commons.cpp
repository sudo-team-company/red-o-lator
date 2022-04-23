#include "runtime-commons.h"

IcdDispatchTable* kDispatchTable =  // NOLINT(cert-err58-cpp)
    IcdDispatchTableProvider().get();

DeviceConfigurationParser kDeviceConfigurationParser =
    DeviceConfigurationParser();

utils::Clock* kClock = new utils::SystemClock();  // NOLINT(cert-err58-cpp)

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
                                           std::to_string(param_name) + ".");
    }

    auto [result, resultSize, isPointer] = parameterValue.value();

    if (std::holds_alternative<std::string>(result)) {
        resultSize = strlen(std::get<std::string>(result).c_str()) + 1;
    }

    if (param_value_size && param_value_size < resultSize) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "Not enough size to fit parameter: need " +
                         std::to_string(resultSize) + ", got " +
                         std::to_string(param_value_size) + ".");
    }

    if (param_value) {
        if (std::holds_alternative<void*>(result)) {
            const auto value = std::get<void*>(result);
            memcpy(param_value, isPointer ? value : &value, resultSize);

        } else {
            memcpy(param_value, std::get<std::string>(result).c_str(),
                   resultSize);
        }
    }

    if (param_value_size_ret) {
        *param_value_size_ret = resultSize;
    }

    kLogger.temp("getParamInfo done");

    return CL_SUCCESS;
}

void enqueueCommand(cl_command_queue queue,
                    cl_uint waitListEventsCount,
                    const cl_event* waitList,
                    cl_event* eventOut,
                    const std::function<Command*()>& commandGetter) {
    const auto command = commandGetter();

    const auto event =
        new CLEvent(kDispatchTable, queue->context, kClock, command);
    command->setEvent(event);
    command->addEventsToWaitList(waitListEventsCount, waitList);

    queue->enqueue(command);

    if (eventOut) {
        *eventOut = event;
    }
}

void registerCall(const std::string& funcName) {
    kLogger.temp("Call " + funcName);
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
