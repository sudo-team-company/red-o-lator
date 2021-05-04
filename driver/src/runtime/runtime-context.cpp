#include <cstring>
#include <iostream>
#include <unordered_map>

#include "common/common.hpp"
#include "icd.h"
#include "runtime-commons.h"

CL_API_ENTRY cl_context CL_API_CALL
clCreateContext(const cl_context_properties* properties,
                cl_uint num_devices,
                const cl_device_id* devices,
                CLContextCallback pfn_notify,
                void* user_data,
                cl_int* errcode_ret) {
    cl_int error = 0;

    if (!num_devices || !devices) {
        error = CL_INVALID_VALUE;
    }

    if (!pfn_notify && user_data) {
        error = CL_INVALID_VALUE;
    }

    if (devices[0] != kDevice) {
        error = CL_INVALID_DEVICE;
    }

    if (error) {
        if (errcode_ret) {
            *errcode_ret = error;
        }

        return nullptr;
    }

    auto clContext = new CLContext(kDispatchTable, devices[0]);

    std::unordered_map<cl_context_properties, cl_context_properties>
        passedProperties;

    if (properties) {
        for (int i = 0; properties[i] != 0; i += 2) {
            passedProperties.emplace(properties[i], properties[i + 1]);
        }

        // TODO(clCreateContext, future): parse props
    }

    clContext->callback = utils::optionalOf(pfn_notify);
    clContext->callbackUserData = user_data;

    if (errcode_ret) {
        *errcode_ret = CL_SUCCESS;
    }

    return clContext;
}

CL_API_ENTRY cl_context CL_API_CALL
clCreateContextFromType(const cl_context_properties* properties,
                        cl_device_type device_type,
                        CLContextCallback pfn_notify,
                        void* user_data,
                        cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateContextFromType"
              << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainContext(cl_context context) {
    if (!context) {
        return CL_INVALID_CONTEXT;
    }

    context->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseContext(cl_context context) {
    if (!context) {
        return CL_INVALID_CONTEXT;
    }

    context->referenceCount--;

    if (context->referenceCount == 0) {
        delete context;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clGetContextInfo(cl_context context,
                                                 cl_context_info param_name,
                                                 size_t param_value_size,
                                                 void* param_value,
                                                 size_t* param_value_size_ret) {
    if (!context) {
        return CL_INVALID_CONTEXT;
    }

    void* result;
    size_t resultSize;

    switch (param_name) {
        case CL_CONTEXT_REFERENCE_COUNT: {
            resultSize = sizeof(cl_uint);
            result = reinterpret_cast<void*>(context->referenceCount);
            break;
        }

        case CL_CONTEXT_NUM_DEVICES: {
            resultSize = sizeof(cl_uint);
            result = reinterpret_cast<void*>(1);
            break;
        }

        case CL_CONTEXT_DEVICES: {
            resultSize = sizeof(cl_device_info);
            result = kDevice;
            break;
        }

        case CL_CONTEXT_PROPERTIES: {
            // TODO(clGetContextInfo): parse props
            resultSize = sizeof(cl_context_properties);
            result = nullptr;
            break;
        }

        default: return CL_INVALID_VALUE;
    }

    if (param_value_size < resultSize) {
        return CL_INVALID_VALUE;
    }

    if (param_value) {
        memcpy(param_value, &result, resultSize);
    }

    if (param_value_size_ret) {
        *param_value_size_ret = resultSize;
    }

    return CL_SUCCESS;
}
