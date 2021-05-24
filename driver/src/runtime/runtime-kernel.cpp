#include <common/utils/common.hpp>
#include <iostream>

#include "runtime-commons.h"
#include "icd/CLCommandQueue.h"
#include "icd/CLProgram.hpp"
#include "command/Command.h"

CL_API_ENTRY cl_kernel CL_API_CALL clCreateKernel(cl_program program,
                                                  const char* kernel_name,
                                                  cl_int* errcode_ret) {
    if (!program) {
        SET_ERROR_AND_RETURN(CL_INVALID_PROGRAM, "Program is null.");
    }

    if (!kernel_name) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE, "Kernel name is null.");
    }

    for (auto* kernel : program->disassembledBinary->kernels) {
        if (kernel->name == kernel_name) {
            kernel->program = program;
            kernel->referenceCount++;
            clRetainProgram(program);

            SET_SUCCESS();

            return kernel;
        }
    }

    SET_ERROR_AND_RETURN(
        CL_INVALID_KERNEL_NAME,
        "Kernel with name " + std::string(kernel_name) + " not found.");
}

CL_API_ENTRY cl_int CL_API_CALL
clCreateKernelsInProgram(cl_program program,
                         cl_uint num_kernels,
                         cl_kernel* kernels,
                         cl_uint* num_kernels_ret) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
    }

    std::cerr << "Unimplemented OpenCL API call: clCreateKernelsInProgram"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainKernel(cl_kernel kernel) {
    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.");
    }

    kernel->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseKernel(cl_kernel kernel) {
    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.");
    }

    kernel->referenceCount--;

    if (kernel->referenceCount == 0) {
        delete kernel;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clSetKernelArg(cl_kernel kernel,
                                               cl_uint arg_index,
                                               size_t arg_size,
                                               const void* arg_value) {
    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.");
    }

    try {
        kernel->setArgument(arg_index, arg_size, arg_value);
    } catch (const KernelArgumentOutOfBoundsError& e) {
        RETURN_ERROR(CL_INVALID_ARG_INDEX, e.what());
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNDRangeKernel(cl_command_queue command_queue,
                       cl_kernel kernel,
                       cl_uint work_dim,
                       const size_t* global_work_offset,
                       const size_t* global_work_size,
                       const size_t* local_work_size,
                       cl_uint num_events_in_wait_list,
                       const cl_event* event_wait_list,
                       cl_event* event) {
    const auto command = std::make_shared<KernelExecutionCommand>(
        kernel, work_dim, global_work_offset, global_work_size,
        local_work_size);

    command_queue->enqueue(command);

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clEnqueueTask(cl_command_queue command_queue,
                                              cl_kernel kernel,
                                              cl_uint num_events_in_wait_list,
                                              const cl_event* event_wait_list,
                                              cl_event* event) {
    size_t workSize[1];
    workSize[0] = 1;
    return clEnqueueNDRangeKernel(command_queue, kernel, 1, nullptr, workSize,
                                  workSize, num_events_in_wait_list,
                                  event_wait_list, event);
}

CL_API_ENTRY cl_int CL_API_CALL
clEnqueueNativeKernel(cl_command_queue command_queue,
                      void (*user_func)(void*),
                      void* args,
                      size_t cb_args,
                      cl_uint num_mem_objects,
                      const cl_mem* mem_list,
                      const void** args_mem_loc,
                      cl_uint num_events_in_wait_list,
                      const cl_event* event_wait_list,
                      cl_event* event) {
    std::cerr << "Unimplemented OpenCL API call: clEnqueueNativeKernel"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetKernelInfo(cl_kernel kernel,
                                                cl_kernel_info param_name,
                                                size_t param_value_size,
                                                void* param_value,
                                                size_t* param_value_size_ret) {
    if (!kernel || !kernel->program) {
        RETURN_ERROR(CL_INVALID_KERNEL,
                     "Kernel is null or its program is null.");
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize;

            switch (param_name) {
                case CL_KERNEL_FUNCTION_NAME: {
                    result = kernel->name;
                    break;
                }

                case CL_KERNEL_NUM_ARGS: {
                    resultSize = sizeof(cl_uint);
                    result = reinterpret_cast<void*>(kernel->argumentCount());
                    break;
                }

                case CL_KERNEL_REFERENCE_COUNT: {
                    resultSize = sizeof(cl_uint);
                    result = reinterpret_cast<void*>(kernel->referenceCount);
                    break;
                }

                case CL_KERNEL_CONTEXT: {
                    resultSize = sizeof(cl_context);
                    result = reinterpret_cast<void*>(kernel->program->context);
                    break;
                }

                case CL_KERNEL_PROGRAM: {
                    resultSize = sizeof(cl_program);
                    result = reinterpret_cast<void*>(kernel->program);
                    break;
                }

                case CL_KERNEL_ATTRIBUTES: {
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
}

CL_API_ENTRY cl_int CL_API_CALL
clGetKernelArgInfo(cl_kernel kernel,
                   cl_uint arg_indx,
                   cl_kernel_arg_info param_name,
                   size_t param_value_size,
                   void* param_value,
                   size_t* param_value_size_ret) {
    if (!kernel || !kernel->program) {
        RETURN_ERROR(CL_INVALID_KERNEL,
                     "Kernel is null or its program is null.");
    }

    if (arg_indx > kernel->argumentCount()) {
        RETURN_ERROR(CL_INVALID_ARG_INDEX,
                     "Index " + std::to_string(arg_indx) +
                         " is more than argument count which is " +
                         std::to_string(kernel->argumentCount()));
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            const auto argumentInfo = kernel->getArgument(arg_indx).info;

            CLObjectInfoParameterValueType result;
            size_t resultSize;

            switch (param_name) {
                case CL_KERNEL_ARG_ADDRESS_QUALIFIER: {
                    resultSize = sizeof(cl_kernel_arg_address_qualifier);

                    if (auto casted = std::dynamic_pointer_cast<
                            PointerKernelArgumentInfo>(argumentInfo)) {
                        result =
                            reinterpret_cast<void*>(casted->addressQualifier);
                    } else {
                        result = reinterpret_cast<void*>(
                            CL_KERNEL_ARG_ADDRESS_PRIVATE);
                    }

                    break;
                }

                case CL_KERNEL_ARG_ACCESS_QUALIFIER: {
                    resultSize = sizeof(cl_kernel_arg_access_qualifier);

                    if (auto casted =
                            std::dynamic_pointer_cast<ImageKernelArgumentInfo>(
                                argumentInfo)) {
                        result =
                            reinterpret_cast<void*>(casted->accessQualifier);
                    } else {
                        result =
                            reinterpret_cast<void*>(CL_KERNEL_ARG_ACCESS_NONE);
                    }

                    break;
                }

                case CL_KERNEL_ARG_TYPE_NAME: {
                    result = argumentInfo->type;
                    break;
                }

                case CL_KERNEL_ARG_TYPE_QUALIFIER: {
                    if (auto casted = std::dynamic_pointer_cast<
                            PointerKernelArgumentInfo>(argumentInfo)) {
                        result =
                            reinterpret_cast<void*>(casted->argTypeQualifier);
                    } else {
                        result =
                            reinterpret_cast<void*>(CL_KERNEL_ARG_TYPE_NONE);
                    }
                    break;
                }

                case CL_KERNEL_ARG_NAME: {
                    result = argumentInfo->argumentName;
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
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
