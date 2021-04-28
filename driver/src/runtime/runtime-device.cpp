#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_int CL_API_CALL clGetDeviceIDs(cl_platform_id platform,
                                               cl_device_type device_type,
                                               cl_uint num_entries,
                                               cl_device_id* devices,
                                               cl_uint* num_devices) {
    std::cerr << "Unimplemented OpenCL API call: clGetDeviceIDs" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetDeviceInfo(cl_device_id device,
                                                cl_device_info param_name,
                                                size_t param_value_size,
                                                void* param_value,
                                                size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetDeviceInfo" << std::endl;
    return CL_INVALID_PLATFORM;
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
