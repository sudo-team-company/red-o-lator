#include <iostream>

#include <program/KernelArgumentInfoParser.h>
#include <common/common.hpp>

#include "icd/CLProgram.hpp"
#include "runtime-commons.h"

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource(cl_context context,
                          cl_uint count,
                          const char** strings,
                          const size_t* lengths,
                          cl_int* errcode_ret) {
    SET_ERROR_AND_RETURN(CL_INVALID_OPERATION,
                         "Only creating programs from binary is supported");
}

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBinary(cl_context context,
                          cl_uint num_devices,
                          const cl_device_id* device_list,
                          const size_t* lengths,
                          const unsigned char** binaries,
                          cl_int* binary_status,
                          cl_int* errcode_ret) {
    if (!device_list || !num_devices) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE,
                             "device_list is null or num_devices == 0.")
    }

    if (!lengths || !binaries) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE,
                             "Source lengths or binaries is null.")
    }

    const auto program = new CLProgram(kDispatchTable, context);

    program->binarySize = lengths[0];
    program->binary = binaries[0];

    SET_SUCCESS()

    return program;
}

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBuiltInKernels(cl_context context,
                                  cl_uint num_devices,
                                  const cl_device_id* device_list,
                                  const char* kernel_names,
                                  cl_int* errcode_ret) {
    std::cerr
        << "Unimplemented OpenCL API call: clCreateProgramWithBuiltInKernels"
        << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainProgram(cl_program program) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.")
    }

    program->referenceCount++;
    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseProgram(cl_program program) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.")
    }

    program->referenceCount--;

    if (program->referenceCount == 0) {
        delete program;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clBuildProgram(cl_program program,
                                               cl_uint num_devices,
                                               const cl_device_id* device_list,
                                               const char* options,
                                               void (*pfn_notify)(cl_program,
                                                                  void*),
                                               void* user_data) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
    }

    program->buildStatus = CL_BUILD_IN_PROGRESS;

    const auto disassembler = BinaryDisassembler();
    try {
        program->disassembledBinary =
            disassembler.disassemble(program->binarySize, program->binary);
        program->buildStatus = CL_BUILD_SUCCESS;
        program->buildLog = "Success disassembling binary";

    } catch (const KernelArgumentInfoParseError& e) {
        kLogger.error(e.what());
        program->buildStatus = CL_BUILD_ERROR;
        program->buildLog = e.what();
        return CL_INVALID_BINARY;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clGetProgramInfo(cl_program program,
                                                 cl_program_info param_name,
                                                 size_t param_value_size,
                                                 void* param_value,
                                                 size_t* param_value_size_ret) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.")
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize;

            switch (param_name) {
                case CL_PROGRAM_REFERENCE_COUNT: {
                    resultSize = sizeof(cl_uint);
                    result = reinterpret_cast<void*>(program->referenceCount);
                    break;
                }

                case CL_PROGRAM_CONTEXT: {
                    resultSize = sizeof(cl_context);
                    result = reinterpret_cast<void*>(program->context);
                    break;
                }

                case CL_PROGRAM_NUM_DEVICES: {
                    resultSize = sizeof(cl_uint);
                    result = reinterpret_cast<void*>(1);
                    break;
                }

                case CL_PROGRAM_DEVICES: {
                    resultSize = sizeof(cl_device_id);
                    result = kDevice;
                    break;
                }

                case CL_PROGRAM_SOURCE: {
                    result = "";
                    break;
                }

                case CL_PROGRAM_BINARY_SIZES: {
                    resultSize = sizeof(size_t);
                    result = reinterpret_cast<void*>(program->binarySize);
                    break;
                }

                case CL_PROGRAM_BINARIES: {
                    resultSize = program->binarySize;
                    result = const_cast<unsigned char*>(program->binary);
                    break;
                }

                case CL_PROGRAM_NUM_KERNELS: {
                    resultSize = sizeof(size_t);
                    result = reinterpret_cast<void*>(0);
                    break;
                }

                case CL_PROGRAM_KERNEL_NAMES: {
                    result = "";
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
}

CL_API_ENTRY cl_int CL_API_CALL
clGetProgramBuildInfo(cl_program program,
                      cl_device_id device,
                      cl_program_build_info param_name,
                      size_t param_value_size,
                      void* param_value,
                      size_t* param_value_size_ret) {
    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.")
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize;

            switch (param_name) {
                case CL_PROGRAM_BUILD_STATUS: {
                    resultSize = sizeof(cl_build_status);
                    result = reinterpret_cast<void*>(program->buildStatus);
                    break;
                }

                case CL_PROGRAM_BUILD_OPTIONS: {
                    result = "";
                    break;
                }

                case CL_PROGRAM_BUILD_LOG: {
                    result = program->buildLog;
                    break;
                }

                case CL_PROGRAM_BINARY_TYPE: {
                    resultSize = sizeof(cl_program_binary_type);
                    result = reinterpret_cast<void*>(
                        CL_PROGRAM_BINARY_TYPE_COMPILED_OBJECT);
                    break;
                }
                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
}

CL_API_ENTRY cl_int CL_API_CALL
clCompileProgram(cl_program program,
                 cl_uint num_devices,
                 const cl_device_id* device_list,
                 const char* options,
                 cl_uint num_input_headers,
                 const cl_program* input_headers,
                 const char** header_include_names,
                 void (*pfn_notify)(cl_program, void*),
                 void* user_data) {
    std::cerr << "Unimplemented OpenCL API call: clCompileProgram" << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_program CL_API_CALL
clLinkProgram(cl_context context,
              cl_uint num_devices,
              const cl_device_id* device_list,
              const char* options,
              cl_uint num_input_programs,
              const cl_program* input_programs,
              void (*pfn_notify)(cl_program, void*),
              void* user_data,
              cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clLinkProgram" << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL
clUnloadPlatformCompiler(cl_platform_id platform) {
    std::cerr << "Unimplemented OpenCL API call: clUnloadPlatformCompiler"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clUnloadCompiler(void) {
    std::cerr << "Unimplemented OpenCL API call: clUnloadCompiler" << std::endl;
    return CL_INVALID_PLATFORM;
}
