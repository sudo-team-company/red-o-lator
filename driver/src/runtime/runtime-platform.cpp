#include <common/common.hpp>
#include <cstring>
#include <iostream>
#include <optional>

#include "icd/icd.h"
#include "runtime-commons.h"

CL_API_ENTRY cl_int CL_API_CALL clGetPlatformIDs(cl_uint num_entries,
                                                 cl_platform_id* platforms,
                                                 cl_uint* num_platforms) {
    if (!platforms && !num_platforms) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "platforms is null and num_platforms is null.")
    }

    if (platforms && num_entries == 0) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "platforms is not null and num_entries == 0.")
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
        RETURN_ERROR(CL_INVALID_PLATFORM, "Platform is null or not valid.")
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            switch (param_name) {
                case CL_PLATFORM_PROFILE: {
                    result = platform->profile;
                    break;
                }

                case CL_PLATFORM_VERSION: {
                    result = platform->openClVersion + " " + platform->name;
                    break;
                }

                case CL_PLATFORM_NAME: {
                    result = platform->name;
                    break;
                }

                case CL_PLATFORM_VENDOR: {
                    result = platform->vendor;
                    break;
                }

                case CL_PLATFORM_EXTENSIONS: {
                    result = platform->extensions;
                    break;
                }

                case CL_PLATFORM_ICD_SUFFIX_KHR: {
                    result = platform->suffix;
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(CLObjectInfoParameterValue(result, 0));
        });
}

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


CL_API_ENTRY void* CL_API_CALL clGetExtensionFunctionAddressForPlatform(
    cl_platform_id platform, const char* func_name) {
    return clGetExtensionFunctionAddress(func_name);
}
