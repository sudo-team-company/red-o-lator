#include <cstring>
#include <iostream>

#include "runtime/runtime-commons.h"
#include "icd/CLDeviceId.hpp"

CL_API_ENTRY cl_int CL_API_CALL clGetDeviceIDs(cl_platform_id platform,
                                               cl_device_type device_type,
                                               cl_uint num_entries,
                                               cl_device_id* devices,
                                               cl_uint* num_devices) {
    if (!platform || platform != kPlatform) {
        RETURN_ERROR(CL_INVALID_PLATFORM, "Platform is null or not valid.")
    }

    if (devices && num_entries == 0) {
        RETURN_ERROR(CL_INVALID_VALUE, "Devices is set but num_entries == 0.");
    }

    if (!kDevice) {
        const auto deviceConfigurationFile =
            "/home/newuserkk/Projects/ITMO/thesis/red-o-lator/driver/resources/"
            "rx-570.ini";
        kDeviceConfigurationParser.load(deviceConfigurationFile);

        kDevice = new CLDeviceId(
            kDispatchTable,
            kDeviceConfigurationParser.requireParameter<cl_device_type>(
                CL_DEVICE_TYPE),
            kDeviceConfigurationParser.requireParameter<size_t>(
                CL_DEVICE_GLOBAL_MEM_SIZE),
            kDeviceConfigurationParser.requireParameter<size_t>(
                CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE),
            kDeviceConfigurationParser.requireParameter<size_t>(
                CL_DEVICE_LOCAL_MEM_SIZE));
    }

    // TODO(clGetDeviceIDs, future): handle num_devices

    if (devices) {
        devices[0] = kDevice;
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
    if (device != kDevice) {
        RETURN_ERROR(CL_INVALID_DEVICE, "Device is null or not valid.")
    }

    if (!param_value && !param_value_size_ret) {
        return CL_SUCCESS;
    }

    // TODO(clGetDeviceInfo, future): parameters validation according to
    //  OpenCL spec

    getParamInfo(param_name, param_value_size, param_value,
                 param_value_size_ret, [&]() {
                     return kDeviceConfigurationParser.getParameter(param_name);
                 });
}

CL_API_ENTRY cl_int CL_API_CALL
clCreateSubDevices(cl_device_id in_device,
                   const cl_device_partition_property* properties,
                   cl_uint num_devices,
                   cl_device_id* out_devices,
                   cl_uint* num_devices_ret) {
    // TODO(clCreateSubDevices): sub-devices support
    std::cerr << "clCreateSubDevices: sub-devices are not supported!"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainDevice(cl_device_id device) {
    // TODO(clRetainDevice): sub-devices support
    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseDevice(cl_device_id device) {
    // TODO(clReleaseDevice): sub-devices support
    return CL_SUCCESS;
}
