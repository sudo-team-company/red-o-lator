#include <iostream>

#include <program/KernelArgumentInfoParser.h>
#include <common/utils/common.hpp>

#include "icd/CLProgram.hpp"
#include "runtime/common/runtime-commons.h"

std::string constructProgramName(int counter, const std::string& ext = "") {
    auto counterStr = std::to_string(counter);
    if (counter < 10) {
        counterStr = "0" + counterStr;
    }
    return "red-o-lator/program" + counterStr + ext;
}

std::string findAvailableProgramName(const std::string& ext = "") {
    int fileNameCounter = 0;

    while (utils::fileExists(constructProgramName(fileNameCounter, ext))) {
        fileNameCounter++;
    }

    return constructProgramName(fileNameCounter, ext);
}

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithSource(cl_context context,
                          cl_uint count,
                          const char** strings,
                          const size_t* lengths,
                          cl_int* errcode_ret) {
    registerCall(__func__);

    if (!context) {
        SET_ERROR_AND_RETURN(CL_INVALID_CONTEXT, "Context is null.");
    }

    if (!strings || !count) {
        SET_ERROR_AND_RETURN(CL_INVALID_VALUE,
                             "Either count, strings or lengths are null.");
    }

    const auto text = strings[0];

    try {
        utils::writeFile(findAvailableProgramName(".cl"), text);
    } catch (const std::runtime_error& e) {
        kLogger.debug(e.what());
    }

    SET_ERROR_AND_RETURN(CL_INVALID_OPERATION,
                         "Only creating programs from binary is supported.");
}

#define SET_BINARY_STATUS(value)      \
    do {                              \
        if (binary_status) {          \
            binary_status[0] = value; \
        }                             \
    } while (0)

#define SET_BINARY_STATUS_AND_RETURN(value, message) \
    do {                                             \
        SET_BINARY_STATUS(value);                    \
        SET_ERROR_AND_RETURN(value, message);        \
    } while (0)

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBinary(cl_context context,
                          cl_uint num_devices,
                          const cl_device_id* device_list,
                          const size_t* lengths,
                          const unsigned char** binaries,
                          cl_int* binary_status,
                          cl_int* errcode_ret) {
    registerCall(__func__);

    if (!device_list || !num_devices) {
        SET_BINARY_STATUS_AND_RETURN(
            CL_INVALID_VALUE, "device_list is null or num_devices == 0.");
    }

    if (!lengths || !binaries) {
        SET_BINARY_STATUS_AND_RETURN(CL_INVALID_VALUE,
                                     "Source lengths or binaries is null.");
    }

    const auto program = new CLProgram(kDispatchTable, context);

    program->binarySize = lengths[0];

    const auto buffer = new std::byte[program->binarySize];
    memcpy(buffer, binaries[0], program->binarySize);
    program->binary = buffer;

    const auto disassembler = BinaryDisassembler();
    try {
        program->disassembledBinary =
            disassembler.disassemble(program->binarySize, program->binary);
        program->buildLog = "Success disassembling binary";

    } catch (const std::runtime_error& e) {
        program->buildLog = e.what();
        SET_BINARY_STATUS_AND_RETURN(CL_INVALID_BINARY, e.what());
    }

    try {
        utils::writeBinaryFile(findAvailableProgramName(".bin"),
                               program->binary, program->binarySize);
        utils::writeFile(findAvailableProgramName(".asm"),
                         program->disassembledBinary->rawOutput);
    } catch (const std::runtime_error& e) {
        kLogger.debug(e.what());
    }

    SET_BINARY_STATUS(CL_SUCCESS);
    SET_SUCCESS();

    return program;
}

CL_API_ENTRY cl_program CL_API_CALL
clCreateProgramWithBuiltInKernels(cl_context context,
                                  cl_uint num_devices,
                                  const cl_device_id* device_list,
                                  const char* kernel_names,
                                  cl_int* errcode_ret) {
    registerCall(__func__);

    std::cerr
        << "Unimplemented OpenCL API call: clCreateProgramWithBuiltInKernels"
        << std::endl;
    return nullptr;
}

CL_API_ENTRY cl_int CL_API_CALL clBuildProgram(cl_program program,
                                               cl_uint num_devices,
                                               const cl_device_id* device_list,
                                               const char* options,
                                               void (*pfn_notify)(cl_program,
                                                                  void*),
                                               void* user_data) {
    registerCall(__func__);

    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
    }

    program->buildStatus = CL_BUILD_SUCCESS;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainProgram(cl_program program) {
    registerCall(__func__);

    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
    }

    program->referenceCount++;
    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseProgram(cl_program program) {
    registerCall(__func__);

    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
    }

    program->referenceCount--;

    if (program->referenceCount == 0) {
        delete program;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clGetProgramInfo(cl_program program,
                                                 cl_program_info param_name,
                                                 size_t param_value_size,
                                                 void* param_value,
                                                 size_t* param_value_size_ret) {
    registerCall(__func__);

    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
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
                    resultSize = 0;
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
                    result = (unsigned char*) program->binary;
                    // TODO: test it
                    break;
                }

                case CL_PROGRAM_NUM_KERNELS: {
                    resultSize = sizeof(size_t);
                    result = reinterpret_cast<void*>(0);
                    break;
                }

                case CL_PROGRAM_KERNEL_NAMES: {
                    resultSize = 0;
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
    registerCall(__func__);

    if (!program) {
        RETURN_ERROR(CL_INVALID_PROGRAM, "Program is null.");
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
                    resultSize = 0;
                    result = "";
                    break;
                }

                case CL_PROGRAM_BUILD_LOG: {
                    resultSize = 0;
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
    registerCall(__func__);

    RETURN_ERROR(CL_INVALID_OPERATION,
                 "clCompileProgram: Compiler is not available.");
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
    registerCall(__func__);

    SET_ERROR_AND_RETURN(CL_INVALID_OPERATION,
                         "clLinkProgram: Linker is not available.");
}

CL_API_ENTRY cl_int CL_API_CALL
clUnloadPlatformCompiler(cl_platform_id platform) {
    registerCall(__func__);

    RETURN_ERROR(CL_INVALID_OPERATION,
                 "clUnloadPlatformCompiler: Compiler is not available.");
}

CL_API_ENTRY cl_int CL_API_CALL clUnloadCompiler() {
    registerCall(__func__);

    RETURN_ERROR(CL_INVALID_OPERATION,
                 "clUnloadCompiler: Compiler is not available.");
}
