#include <common/common.hpp>
#include <iostream>

#include "icd/CLCommandQueue.h"
#include "runtime-commons.h"

CL_API_ENTRY cl_command_queue CL_API_CALL
clCreateCommandQueue(cl_context context,
                     cl_device_id device,
                     cl_command_queue_properties properties,
                     cl_int* errcode_ret) {
    if (!context) {
        SET_ERROR_AND_RETURN(CL_INVALID_CONTEXT, "Context is null.")
    }

    if (device != kDevice) {
        SET_ERROR_AND_RETURN(CL_INVALID_DEVICE, "Device is null or not valid.")
    }

    if (properties & CL_QUEUE_PROFILING_ENABLE) {
        // TODO(clCreateCommandQueue): profiling support
        SET_ERROR_AND_RETURN(CL_INVALID_QUEUE_PROPERTIES,
                             "Profiling is not supported yet.")
    }

    if (properties & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE) {
        // TODO(clCreateCommandQueue): out-of-order exec support
        SET_ERROR_AND_RETURN(
            CL_INVALID_QUEUE_PROPERTIES,
            "Out-of-order execution mode is not supported yet.")
    }

    const auto commandQueue =
        new CLCommandQueue(kDispatchTable, context, properties);

    SET_SUCCESS()

    return commandQueue;
}

CL_API_ENTRY cl_int CL_API_CALL clFinish(cl_command_queue command_queue) {
    if (!command_queue) {
        RETURN_ERROR(CL_INVALID_COMMAND_QUEUE, "Command queue is null.")
    }

    command_queue->flush();

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clFlush(cl_command_queue command_queue) {
    return clFinish(command_queue);
}

CL_API_ENTRY cl_int CL_API_CALL
clRetainCommandQueue(cl_command_queue command_queue) {
    if (!command_queue) {
        RETURN_ERROR(CL_INVALID_COMMAND_QUEUE, "Command queue is null.")
    }

    command_queue->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseCommandQueue(cl_command_queue command_queue) {
    if (!command_queue) {
        RETURN_ERROR(CL_INVALID_COMMAND_QUEUE, "Command queue is null.")
    }

    clFlush(command_queue);

    command_queue->referenceCount--;

    if (command_queue->referenceCount == 0) {
        delete command_queue;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetCommandQueueInfo(cl_command_queue command_queue,
                      cl_command_queue_info param_name,
                      size_t param_value_size,
                      void* param_value,
                      size_t* param_value_size_ret) {
    if (!command_queue) {
        RETURN_ERROR(CL_INVALID_COMMAND_QUEUE, "Command queue is null.")
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize;
            switch (param_name) {
                case CL_QUEUE_CONTEXT: {
                    resultSize = sizeof(cl_context);
                    result = reinterpret_cast<void*>(command_queue->context);
                    break;
                }

                case CL_QUEUE_DEVICE: {
                    resultSize = sizeof(cl_device_id);
                    result =
                        reinterpret_cast<void*>(command_queue->context->device);
                    break;
                }

                case CL_QUEUE_REFERENCE_COUNT: {
                    resultSize = sizeof(cl_uint);
                    result =
                        reinterpret_cast<void*>(command_queue->referenceCount);
                    break;
                }

                case CL_QUEUE_PROPERTIES: {
                    resultSize = sizeof(cl_command_queue_properties);
                    result =
                        reinterpret_cast<void*>(command_queue->properties);
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
}
