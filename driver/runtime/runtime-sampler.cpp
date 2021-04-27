#include <opencl.h>
#include <iostream>

CL_API_ENTRY cl_int CL_API_CALL clRetainSampler(cl_sampler sampler) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseSampler(cl_sampler sampler) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetSamplerInfo(cl_sampler sampler,
                                                 cl_sampler_info param_name,
                                                 size_t param_value_size,
                                                 void* param_value,
                                                 size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_2_DEPRECATED cl_sampler CL_API_CALL
clCreateSampler(cl_context context,
                cl_bool normalized_coords,
                cl_addressing_mode addressing_mode,
                cl_filter_mode filter_mode,
                cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}
