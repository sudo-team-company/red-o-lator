#include <cstring>
#include <iostream>
#include <variant>

#include "runtime/runtime-commons.h"

CL_API_ENTRY cl_int CL_API_CALL clGetDeviceIDs(cl_platform_id platform,
                                               cl_device_type device_type,
                                               cl_uint num_entries,
                                               cl_device_id* devices,
                                               cl_uint* num_devices) {
    if (devices) {
        const auto deviceConfigurationFile =
            "/home/newuserkk/Projects/ITMO/thesis/red-o-lator/driver/resources/"
            "rx-570.ini";
        kDeviceConfigurationParser.load(deviceConfigurationFile);

        const auto device = new CLDeviceId();
        device->dispatchTable = kDispatchTable;
        devices[0] = device;
    }

    if (num_devices) {
        *num_devices = 1;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clGetDeviceInfo(cl_device_id device,
                                                cl_device_info param_name,
                                                size_t param_value_size,
                                                void* param_value,
                                                size_t* param_value_size_ret) {
    if (!param_value && !param_value_size_ret) {
        return CL_SUCCESS;
    }

    if (!param_value) {
        *param_value_size_ret = 0;
        return CL_SUCCESS;
    }

    // TODO: parameters validation according to OpenCL spec
    const auto maybeResult =
        kDeviceConfigurationParser.getParameter(param_name);

    if (!maybeResult) {
        return CL_INVALID_VALUE;
    }

    const auto& [result, resultSize] = maybeResult.value();

    if (param_value_size < resultSize) {
        return CL_INVALID_VALUE;
    }

    if (std::holds_alternative<void*>(result)) {
        memcpy(param_value, &std::get<void*>(result), resultSize);
    } else {
        memcpy(param_value, std::get<std::string>(result).c_str(), resultSize);
    }

    if (param_value_size_ret) {
        *param_value_size_ret = resultSize;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clCreateSubDevices(cl_device_id in_device,
                   const cl_device_partition_property* properties,
                   cl_uint num_devices,
                   cl_device_id* out_devices,
                   cl_uint* num_devices_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateSubDevices"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainDevice(cl_device_id device) {
    std::cerr << "Unimplemented OpenCL API call: clRetainDevice" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseDevice(cl_device_id device) {
    std::cerr << "Unimplemented OpenCL API call: clReleaseDevice" << std::endl;
    return CL_INVALID_PLATFORM;
}
