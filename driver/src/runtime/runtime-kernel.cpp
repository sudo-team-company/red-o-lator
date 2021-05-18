#include <common/common.hpp>
#include <iostream>
#include "icd/CLProgram.hpp"
#include "runtime-commons.h"

CL_API_ENTRY cl_kernel CL_API_CALL clCreateKernel(cl_program program,
                                                  const char* kernel_name,
                                                  cl_int* errcode_ret) {
    if (!program) {
        SET_ERROR_AND_RETURN(CL_INVALID_PROGRAM, "Program is null.")
    }

    if (!kernel_name) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE, "Kernel name is null.")
    }

    for (auto* kernel : program->disassembledBinary->kernels) {
        kLogger.debug(kernel->name);
        if (kernel->name == kernel_name) {
            kernel->program = program;
            kernel->referenceCount++;
            clRetainProgram(program);

            SET_SUCCESS()

            return kernel;
        }
    }

    SET_ERROR_AND_RETURN(
        CL_INVALID_KERNEL_NAME,
        "Kernel with name " + std::string(kernel_name) + " not found.")
}

CL_API_ENTRY cl_int CL_API_CALL
clCreateKernelsInProgram(cl_program program,
                         cl_uint num_kernels,
                         cl_kernel* kernels,
                         cl_uint* num_kernels_ret) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.")
    }

    std::cerr << "Unimplemented OpenCL API call: clCreateKernelsInProgram"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainKernel(cl_kernel kernel) {
    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.")
    }

    kernel->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseKernel(cl_kernel kernel) {
    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.")
    }

    kernel->referenceCount--;

    if (kernel->referenceCount == 0) {
        clReleaseProgram(kernel->program);
        delete kernel;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clSetKernelArg(cl_kernel kernel,
                                               cl_uint arg_index,
                                               size_t arg_size,
                                               const void* arg_value) {
    if (!kernel) {
        RETURN_ERROR(CL_INVALID_KERNEL, "Kernel is null.")
    }

    // TODO: parameters validation
    kernel->setArgument(arg_index, arg_size, arg_value);

    return CL_SUCCESS;
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
                     "Kernel is null or its program is null.")
    }

    if (arg_indx > kernel->argumentCount()) {
        RETURN_ERROR(CL_INVALID_ARG_INDEX,
                     "Index " + std::to_string(arg_indx) +
                         " is more than argument count which is " +
                         std::to_string(kernel->argumentCount()))
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
