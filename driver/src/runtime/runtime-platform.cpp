#include <cstring>
#include <iostream>

#include "runtime-commons.h"

CL_API_ENTRY void* CL_API_CALL
clGetExtensionFunctionAddress(const char* func_name) {
    if (strcmp(func_name, "clIcdGetPlatformIDsKHR") == 0) {
        return (void*) clIcdGetPlatformIDsKHR;
    } else if (strcmp(func_name, "clGetPlatformInfo") == 0) {
        return (void*) clGetPlatformInfo;
    } else {
        return nullptr;
    }
}

CL_API_ENTRY cl_int CL_API_CALL clIcdGetPlatformIDsKHR(
    cl_uint num_entries, cl_platform_id* platforms, cl_uint* num_platforms) {
    return clGetPlatformIDs(num_entries, platforms, num_platforms);
}

CL_API_ENTRY cl_int CL_API_CALL clGetPlatformIDs(cl_uint num_entries,
                                                 cl_platform_id* platforms,
                                                 cl_uint* num_platforms) {
    if (!platforms && !num_platforms) {
        return CL_INVALID_VALUE;
    }

    if (platforms && num_entries == 0) {
        return CL_INVALID_VALUE;
    }

    if (!kPlatform) {
        kPlatform = new CLPlatformId(kDispatchTable);
        kPlatform->openClVersion = "OpenCL 1.2";
        kPlatform->driverVersion = "0.1";
        kPlatform->name = "AMD Accelerated Parallel Processing";
        kPlatform->vendor = "sudo-team-company";
        kPlatform->extensions = "cl_khr_icd";
        kPlatform->suffix = "red-o-lator";
        kPlatform->profile = "FULL_PROFILE";
    }

    if (platforms) {
        platforms[0] = kPlatform;
    }

    if (num_platforms) {
        *num_platforms = 1;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetPlatformInfo(cl_platform_id platform,
                  cl_platform_info param_name,
                  size_t param_value_size,
                  void* param_value,
                  size_t* param_value_size_ret) {
    if (platform != kPlatform) {
        return CL_INVALID_PLATFORM;
    }

    std::string returnString;

    switch (param_name) {
        case CL_PLATFORM_PROFILE: {
            returnString = platform->profile;
            break;
        }

        case CL_PLATFORM_VERSION: {
            returnString =
                platform->openClVersion + std::string(" ") + platform->name;
            break;
        }

        case CL_PLATFORM_NAME: {
            returnString = platform->name;
            break;
        }

        case CL_PLATFORM_VENDOR: {
            returnString = platform->vendor;
            break;
        }

        case CL_PLATFORM_EXTENSIONS: {
            returnString = platform->extensions;
            break;
        }

        case CL_PLATFORM_ICD_SUFFIX_KHR: {
            returnString = platform->suffix;
            break;
        }

        default: return CL_INVALID_VALUE;
    }

    const auto cReturnString = returnString.c_str();
    const auto returnStringSize = strlen(cReturnString) + 1;

    if (param_value_size && param_value_size < returnStringSize) {
        return CL_INVALID_VALUE;
    }

    if (param_value) {
        memcpy(param_value, cReturnString, returnStringSize);
    }

    if (param_value_size_ret) {
        *param_value_size_ret = returnStringSize;
    }

    return CL_SUCCESS;
}
