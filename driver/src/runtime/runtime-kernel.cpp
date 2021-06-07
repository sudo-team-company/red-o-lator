#include <common/utils/common.hpp>
#include <iostream>

#include "command/Command.h"
#include "icd/CLCommandQueue.h"
#include "icd/CLProgram.hpp"
#include "runtime/common/runtime-commons.h"

CL_API_ENTRY cl_kernel CL_API_CALL clCreateKernel(cl_program program,
                                                  const char* kernel_name,
                                                  cl_int* errcode_ret) {
    registerCall(__func__);

    if (!program) {
        SET_ERROR_AND_RETURN(CL_INVALID_PROGRAM, "Program is null.");
    }

    if (!kernel_name) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE, "Kernel name is null.");
    }

    for (auto& kernelBuilder : program->disassembledBinary->kernelBuilders) {
        if (kernelBuilder->name == kernel_name) {
            const auto kernel = kernelBuilder->build();
            kernel->program = program;
            program->createdKernels.push_back(kernel);
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
    registerCall(__func__);

    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
    }

    const auto& disassembledKernels = program->disassembledBinary->kernelBuilders;

    if (kernels && num_kernels < disassembledKernels.size()) {
        RETURN_ERROR(
            CL_INVALID_VALUE,
            "num_kernels is less than total number of kernels which is " +
                std::to_string(disassembledKernels.size()) + ".");
    }

    if (kernels) {
        for (int i = 0; i < disassembledKernels.size(); ++i) {
            auto kernel = disassembledKernels[i]->build();
            kernel->program = program;
            program->createdKernels.push_back(kernel);
            clRetainProgram(program);

            kernels[i] = kernel;
        }
    }

    if (num_kernels_ret) {
        *num_kernels_ret = disassembledKernels.size();
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clSetKernelArg(cl_kernel kernel,
                                               cl_uint arg_index,
                                               size_t arg_size,
                                               const void* arg_value) {
    registerCall(__func__);

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

CL_API_ENTRY cl_int CL_API_CALL clRetainKernel(cl_kernel kernel) {
    registerCall(__func__);

    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.");
    }

    kernel->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseKernel(cl_kernel kernel) {
    registerCall(__func__);

    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.");
    }

    kernel->referenceCount--;

    if (kernel->referenceCount == 0) {
        delete kernel;
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
    registerCall(__func__);

    if (!command_queue) {
        RETURN_ERROR(CL_INVALID_COMMAND_QUEUE, "Command queue is null.");
    }

    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.");
    }

    if (work_dim < 1 || work_dim > 3) {
        RETURN_ERROR(CL_INVALID_WORK_DIMENSION,
                     "Work dimension should be in range from 1 to 3, got " +
                         std::to_string(work_dim) + ".");
    }

    if (!global_work_size) {
        RETURN_ERROR(CL_INVALID_GLOBAL_WORK_SIZE, "Global work size is null.");
    }

    cl_uint workGroupSize = 1;
    const auto maxWorkGroupSize =
        kDeviceConfigurationParser.requireParameter<const size_t>(
            CL_DEVICE_MAX_WORK_GROUP_SIZE);
    const auto maxWorkItemSizes =
        kDeviceConfigurationParser.requireParameter<const size_t*>(
            CL_DEVICE_MAX_WORK_ITEM_SIZES);
    for (cl_uint i = 0; i < work_dim; ++i) {
        if (!global_work_size[i]) {
            RETURN_ERROR(CL_INVALID_GLOBAL_WORK_SIZE,
                         "Global work size is 0 for dimension " +
                             std::to_string(i) + ".");
        }

        if (local_work_size) {
            workGroupSize *= local_work_size[i];
            if (local_work_size[i] > maxWorkItemSizes[i]) {
                RETURN_ERROR(
                    CL_INVALID_WORK_ITEM_SIZE,
                    "Specified number of work items at the dimension " +
                        std::to_string(i) + "which is " +
                        std::to_string(local_work_size[i]) +
                        " is greater than max allowed of " +
                        std::to_string(maxWorkItemSizes[i]));
            }
        } else {
            local_work_size = maxWorkItemSizes;
        }
    }

    if (local_work_size && workGroupSize > maxWorkGroupSize) {
        RETURN_ERROR(CL_INVALID_WORK_GROUP_SIZE,
                     "Total work group size is greater than maximum supported "
                     "by device: " +
                         std::to_string(workGroupSize) + "/" +
                         std::to_string(maxWorkGroupSize));
    }

    std::function<bool(const KernelArgument&)> argNotSetPredicate =
        [](const KernelArgument& kernel) {
            return !kernel.value.has_value();
        };

    if (std::any_of(kernel->getArguments().begin(),
                    kernel->getArguments().end(), argNotSetPredicate)) {
        RETURN_ERROR(CL_INVALID_KERNEL_ARGS,
                     "Not all kernel arguments are set.");
    }

    enqueueCommand(command_queue, num_events_in_wait_list, event_wait_list,
                   event, [&]() {
                       return new KernelExecutionCommand(
                           command_queue, kernel, work_dim, global_work_offset,
                           global_work_size, local_work_size);
                   });

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clEnqueueTask(cl_command_queue command_queue,
                                              cl_kernel kernel,
                                              cl_uint num_events_in_wait_list,
                                              const cl_event* event_wait_list,
                                              cl_event* event) {
    registerCall(__func__);

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
    registerCall(__func__);

    std::cerr << "Unimplemented OpenCL API call: clEnqueueNativeKernel"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clGetKernelInfo(cl_kernel kernel,
                                                cl_kernel_info param_name,
                                                size_t param_value_size,
                                                void* param_value,
                                                size_t* param_value_size_ret) {
    registerCall(__func__);

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
    registerCall(__func__);

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
    registerCall(__func__);

    if (!kernel || !kernel->program) {
        RETURN_ERROR(CL_INVALID_KERNEL,
                     "Kernel is null or its program is null.");
    }

    if (param_name == CL_KERNEL_GLOBAL_WORK_SIZE) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "CL_KERNEL_GLOBAL_WORK_SIZE works only on custom device "
                     "or built-in kernel.");
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize;
            bool isArray = false;

            switch (param_name) {
                case CL_KERNEL_WORK_GROUP_SIZE: {
                    // TODO: should be determined by emulator
                    resultSize = sizeof(size_t) * 3;
                    auto* data = new size_t[3];
                    data[0] = kernel->requiredWorkGroupSize.x;
                    data[1] = kernel->requiredWorkGroupSize.y;
                    data[2] = kernel->requiredWorkGroupSize.z;
                    result = data;
                    isArray = true;
                    break;
                }

                case CL_KERNEL_COMPILE_WORK_GROUP_SIZE: {
                    resultSize = sizeof(size_t) * 3;
                    auto* data = new size_t[3];
                    data[0] = kernel->requiredWorkGroupSize.x;
                    data[1] = kernel->requiredWorkGroupSize.y;
                    data[2] = kernel->requiredWorkGroupSize.z;
                    result = data;
                    isArray = true;
                    break;
                }

                case CL_KERNEL_LOCAL_MEM_SIZE: {
                    // TODO: proper getter
                    resultSize = sizeof(cl_ulong);
                    result = reinterpret_cast<void*>(0);
                    break;
                }

                case CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE: {
                    // TODO: proper getter
                    resultSize = sizeof(size_t);
                    result = reinterpret_cast<void*>(1);
                    break;
                }

                case CL_KERNEL_PRIVATE_MEM_SIZE: {
                    // TODO: proper getter
                    resultSize = sizeof(cl_ulong);
                    result = reinterpret_cast<void*>(0);
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize, isArray));
        });
}
