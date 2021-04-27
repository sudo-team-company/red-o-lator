#include <opencl.h>
#include <iostream>

CL_API_ENTRY cl_int CL_API_CALL clFlush(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clFinish(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY void* CL_API_CALL clGetExtensionFunctionAddressForPlatform(
    cl_platform_id platform, const char* func_name) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}

CL_API_ENTRY CL_API_SUFFIX__VERSION_1_1_DEPRECATED cl_int CL_API_CALL
clUnloadCompiler(void) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY CL_API_PREFIX__VERSION_1_1_DEPRECATED void* CL_API_CALL
clGetExtensionFunctionAddress(const char* func_name) {
    std::cerr << "Unimplemented OpenCL API call." << std::endl;
    return nullptr;
}