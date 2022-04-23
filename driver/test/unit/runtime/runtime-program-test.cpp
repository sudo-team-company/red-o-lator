#include <common/test/doctest.h>

#include <common/utils/common.hpp>
#include <vector>

#include "runtime/icd/CLProgram.hpp"
#include "runtime/icd/icd.h"
#include "unit-test-common/test-commons.h"

TEST_SUITE("Program API") {
    const auto binaryPath = "test/resources/kernels/a_plus_b.bin";

    TEST_CASE("clCreateProgramWithBinary") {
        SUBCASE("program can be created with valid binary") {
            const auto context = test::getContext();
            const auto contextInitRefCount = context->referenceCount;
            const cl_device_id device[1] = {context->device};

            const auto binary = utils::readBinaryFile(binaryPath);
            const size_t binarySize[1] = {binary.size()};
            const unsigned char* binaryData[1] = {binary.data()};

            cl_int binaryStatus;
            cl_int error;

            const auto program =
                clCreateProgramWithBinary(context, 1, device, binarySize,
                                          binaryData, &binaryStatus, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(binaryStatus == CL_SUCCESS);
            CHECK(program != nullptr);

            CHECK(program->referenceCount == 1);
            CHECK(program->disassembledBinary != nullptr);
            CHECK(program->buildStatus == CL_BUILD_NONE);
            CHECK(program->context->referenceCount == contextInitRefCount + 1);
            CHECK(program->binarySize == binary.size());
            CHECK(program->binary != nullptr);
        }

        SUBCASE("fail if required parameters are null") {
            const auto context = test::getContext();
            const cl_device_id device[1] = {context->device};
            const size_t binarySize[1]{16};
            const unsigned char* binaryData[1];

            cl_int binaryStatus;
            cl_int error;
            cl_program program;
            program =
                clCreateProgramWithBinary(context, 0, device, binarySize,
                                          binaryData, &binaryStatus, &error);
            CHECK(error == CL_INVALID_VALUE);
            CHECK(binaryStatus == CL_INVALID_VALUE);
            CHECK(program == nullptr);

            program =
                clCreateProgramWithBinary(context, 1, nullptr, binarySize,
                                          binaryData, &binaryStatus, &error);
            CHECK(error == CL_INVALID_VALUE);
            CHECK(binaryStatus == CL_INVALID_VALUE);
            CHECK(program == nullptr);

            program = clCreateProgramWithBinary(
                context, 1, device, 0, binaryData, &binaryStatus, &error);
            CHECK(error == CL_INVALID_VALUE);
            CHECK(binaryStatus == CL_INVALID_VALUE);
            CHECK(program == nullptr);

            program = clCreateProgramWithBinary(context, 1, nullptr, binarySize,
                                                nullptr, &binaryStatus, &error);
            CHECK(error == CL_INVALID_VALUE);
            CHECK(binaryStatus == CL_INVALID_VALUE);
            CHECK(program == nullptr);
        }
    }

    TEST_CASE("clBuildProgram") {
        SUBCASE("changes build status") {
            const auto program = test::getProgram(binaryPath);
            const auto error =
                clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
            CHECK(error == CL_SUCCESS);
        }

        SUBCASE("fails with null program") {
            const auto error =
                clBuildProgram(nullptr, 1, nullptr, nullptr, nullptr, nullptr);
            CHECK(error == CL_INVALID_PROGRAM);
        }
    }

    TEST_CASE("clRetainProgram") {
        SUBCASE("increments program ref counter") {
            const auto program = test::getProgram(binaryPath);
            const auto initialRefCount = program->referenceCount;

            const auto error = clRetainProgram(program);

            CHECK(error == CL_SUCCESS);
            CHECK(program->referenceCount == initialRefCount + 1);
        }
    }

    TEST_CASE("clReleaseProgram") {
        SUBCASE("decrements program ref counter") {
            const auto program = test::getProgram(binaryPath);

            clRetainProgram(program);

            const auto initialRefCount = program->referenceCount;

            const auto error = clReleaseProgram(program);

            CHECK(error == CL_SUCCESS);
            CHECK(program->referenceCount == initialRefCount - 1);
        }
    }

    TEST_CASE("clGetProgramInfo") {
        SUBCASE("get reference count") {
            const auto program = test::getProgram(binaryPath);

            cl_uint out;
            const auto error =
                clGetProgramInfo(program, CL_PROGRAM_REFERENCE_COUNT,
                                 sizeof(cl_uint), &out, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(out == program->referenceCount);
        }
    }

    TEST_CASE("clGetProgramBuildInfo") {
        SUBCASE("get build status") {
            const auto program = test::getProgram(binaryPath);

            cl_build_status out;
            const auto error = clGetProgramBuildInfo(
                program, program->context->device, CL_PROGRAM_BUILD_STATUS,
                sizeof(cl_build_status), &out, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(out == program->buildStatus);
        }
    }

    TEST_CASE("clCreateProgramWithSource") {
        SUBCASE("clCreateProgramWithSource is not supported") {
            cl_int error;
            const size_t* lengths = new size_t[1]{2};
            const char** sources = new const char* [1] {
                "a"
            };
            const auto program = clCreateProgramWithSource(
                test::getContext(), 1, sources, lengths, &error);

            CHECK(error == CL_INVALID_OPERATION);
            CHECK(program == nullptr);
        }
    }

    TEST_CASE("clCompileProgram") {
        SUBCASE("clCompileProgram is not supported") {
            const auto error =
                clCompileProgram(nullptr, 0, nullptr, nullptr, 0, nullptr,
                                 nullptr, nullptr, nullptr);
            CHECK(error == CL_INVALID_OPERATION);
        }
    }

    TEST_CASE("clLinkProgram") {
        SUBCASE("clLinkProgram is not supported") {
            cl_int error;
            const auto program =
                clLinkProgram(nullptr, 0, nullptr, nullptr, 0, nullptr, nullptr,
                              nullptr, &error);

            CHECK(error == CL_INVALID_OPERATION);
            CHECK(program == nullptr);
        }
    }

    TEST_CASE("clUnloadPlatformCompiler") {
        SUBCASE("clUnloadPlatformCompiler is not supported") {
            const auto error = clUnloadPlatformCompiler(nullptr);
            CHECK(error == CL_INVALID_OPERATION);
        }
    }

    TEST_CASE("clUnloadCompiler") {
        SUBCASE("clUnloadCompiler is not supported") {
            const auto error = clUnloadCompiler();
            CHECK(error == CL_INVALID_OPERATION);
        }
    }
}