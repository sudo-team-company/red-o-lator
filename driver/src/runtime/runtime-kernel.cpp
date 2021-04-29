#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_kernel CL_API_CALL clCreateKernel(cl_program program,
                                                  const char* kernel_name,
                                                  cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateKernel" << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clCreateKernelsInProgram(cl_program program,
                         cl_uint num_kernels,
                         cl_kernel* kernels,
                         cl_uint* num_kernels_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateKernelsInProgram"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainKernel(cl_kernel kernel) {
    std::cerr << "Unimplemented OpenCL API call: clRetainKernel" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseKernel(cl_kernel kernel) {
    std::cerr << "Unimplemented OpenCL API call: clReleaseKernel" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clSetKernelArg(cl_kernel kernel,
                                               cl_uint arg_index,
                                               size_t arg_size,
                                               const void* arg_value) {
    std::cerr << "Unimplemented OpenCL API call: clSetKernelArg" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetKernelInfo(cl_kernel kernel,
                                                cl_kernel_info param_name,
                                                size_t param_value_size,
                                                void* param_value,
                                                size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetKernelInfo" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelArgInfo(cl_kernel kernel,
                   cl_uint arg_indx,
                   cl_kernel_arg_info param_name,
                   size_t param_value_size,
                   void* param_value,
                   size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetKernelArgInfo"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelWorkGroupInfo(cl_kernel kernel,
                         cl_device_id device,
                         cl_kernel_work_group_info param_name,
                         size_t param_value_size,
                         void* param_value,
                         size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetKernelWorkGroupInfo"
              << std::endl;
    return CL_INVALID_PLATFORM;
}
