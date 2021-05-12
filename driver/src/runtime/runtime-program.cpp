#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource(cl_context context,
                          cl_uint count,
                          const char** strings,
                          const size_t* lengths,
                          cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateProgramWithSource"
              << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBinary(cl_context context,
                          cl_uint num_devices,
                          const cl_device_id* device_list,
                          const size_t* lengths,
                          const unsigned char** binaries,
                          cl_int* binary_status,
                          cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateProgramWithBinary"
              << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBuiltInKernels(cl_context context,
                                  cl_uint num_devices,
                                  const cl_device_id* device_list,
                                  const char* kernel_names,
                                  cl_int* errcode_ret) {
    std::cerr
        << "Unimplemented OpenCL API call: clCreateProgramWithBuiltInKernels"
        << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainProgram(cl_program program) {
    std::cerr << "Unimplemented OpenCL API call: clRetainProgram" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseProgram(cl_program program) {
    std::cerr << "Unimplemented OpenCL API call: clReleaseProgram" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clBuildProgram(cl_program program,
                                               cl_uint num_devices,
                                               const cl_device_id* device_list,
                                               const char* options,
                                               void (*pfn_notify)(cl_program,
                                                                  void*),
                                               void* user_data) {
    std::cerr << "Unimplemented OpenCL API call: clBuildProgram" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clCompileProgram(cl_program program,
                 cl_uint num_devices,
                 const cl_device_id* device_list,
                 const char* options,
                 cl_uint num_input_headers,
                 const cl_program* input_headers,
                 const char** header_include_names,
                 void (*pfn_notify)(cl_program, void*),
                 void* user_data) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_program CL_API_CALL
clLinkProgram(cl_context context,
              cl_uint num_devices,
              const cl_device_id* device_list,
              const char* options,
              cl_uint num_input_programs,
              const cl_program* input_programs,
              void (*pfn_notify)(cl_program, void*),
              void* user_data,
              cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clUnloadPlatformCompiler(cl_platform_id platform) {
    std::cerr << "Unimplemented OpenCL API call: clUnloadPlatformCompiler"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetProgramInfo(cl_program program,
                                                 cl_program_info param_name,
                                                 size_t param_value_size,
                                                 void* param_value,
                                                 size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetProgramInfo" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetProgramBuildInfo(cl_program program,
                      cl_device_id device,
                      cl_program_build_info param_name,
                      size_t param_value_size,
                      void* param_value,
                      size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}
