#include <cstring>
#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_int CL_API_CALL clFlush(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call: clFlush" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clFinish(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call: clFinish" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY void* CL_API_CALL clGetExtensionFunctionAddressForPlatform(
    cl_platform_id platform, const char* func_name) {
    std::cerr << "Unimplemented OpenCL API call: "
                 "clGetExtensionFunctionAddressForPlatform"
              << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clUnloadCompiler(void) {
    std::cerr << "Unimplemented OpenCL API call: clUnloadCompiler" << std::endl;
    return CL_INVALID_PLATFORM;
}
