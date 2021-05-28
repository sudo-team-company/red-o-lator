#include <common/test/doctest.h>

#include <common/utils/vector-utils.hpp>
#include <vector>

#include "runtime/icd/CLProgram.hpp"
#include "runtime/icd/icd.h"
#include "runtime/icd/kernel/CLKernel.h"
#include "unit-test-common/test-commons.h"

TEST_SUITE("Kernel API") {
    const auto binaryPath = "test/resources/kernels/a_plus_b.bin";
    const auto kernelName = "a_plus_b";

    TEST_CASE("clCreateKernel") {
        SUBCASE("can get kernel by name") {
            auto program = test::getProgram(binaryPath);
            const auto programInitRefCount = program->referenceCount;

            cl_int error;
            const auto kernel = clCreateKernel(program, kernelName, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(kernel->referenceCount == 1);
            CHECK(program->referenceCount == programInitRefCount + 1);
        }
    }

    TEST_CASE("clCreateKernelsInProgram") {
        SUBCASE("should return all kernels in program") {
            const auto expected = std::vector<std::string>{
                "add_x_x",
                "add_get_global_offset_x",
                "add_get_local_id_x",
                "add_get_group_id_x",
                "add_get_local_size_x",
                "add_get_global_id_x",
                "add_get_global_size_x",
                "add_get_num_groups_x",
                "add_get_work_dim_x",
                "add_x_get_global_offset",
                "add_get_global_offset_get_global_offset",
                "add_get_local_id_get_global_offset",
                "add_get_group_id_get_global_offset",
                "add_get_local_size_get_global_offset",
                "add_get_global_id_get_global_offset",
                "add_get_global_size_get_global_offset",
                "add_get_num_groups_get_global_offset",
                "add_get_work_dim_get_global_offset",
                "add_x_get_local_id",
                "add_get_global_offset_get_local_id",
                "add_get_local_id_get_local_id",
                "add_get_group_id_get_local_id",
                "add_get_local_size_get_local_id",
                "add_get_global_id_get_local_id",
                "add_get_global_size_get_local_id",
                "add_get_num_groups_get_local_id",
                "add_get_work_dim_get_local_id",
                "add_x_get_group_id",
                "add_get_global_offset_get_group_id",
                "add_get_local_id_get_group_id",
                "add_get_group_id_get_group_id",
                "add_get_local_size_get_group_id",
                "add_get_global_id_get_group_id",
                "add_get_global_size_get_group_id",
                "add_get_num_groups_get_group_id",
                "add_get_work_dim_get_group_id",
                "add_x_get_local_size",
                "add_get_global_offset_get_local_size",
                "add_get_local_id_get_local_size",
                "add_get_group_id_get_local_size",
                "add_get_local_size_get_local_size",
                "add_get_global_id_get_local_size",
                "add_get_global_size_get_local_size",
                "add_get_num_groups_get_local_size",
                "add_get_work_dim_get_local_size",
                "add_x_get_global_id",
                "add_get_global_offset_get_global_id",
                "add_get_local_id_get_global_id",
                "add_get_group_id_get_global_id",
                "add_get_local_size_get_global_id",
                "add_get_global_id_get_global_id",
                "add_get_global_size_get_global_id",
                "add_get_num_groups_get_global_id",
                "add_get_work_dim_get_global_id",
                "add_x_get_global_size",
                "add_get_global_offset_get_global_size",
                "add_get_local_id_get_global_size",
                "add_get_group_id_get_global_size",
                "add_get_local_size_get_global_size",
                "add_get_global_id_get_global_size",
                "add_get_global_size_get_global_size",
                "add_get_num_groups_get_global_size",
                "add_get_work_dim_get_global_size",
                "add_x_get_num_groups",
                "add_get_global_offset_get_num_groups",
                "add_get_local_id_get_num_groups",
                "add_get_group_id_get_num_groups",
                "add_get_local_size_get_num_groups",
                "add_get_global_id_get_num_groups",
                "add_get_global_size_get_num_groups",
                "add_get_num_groups_get_num_groups",
                "add_get_work_dim_get_num_groups",
                "add_x_get_work_dim",
                "add_get_global_offset_get_work_dim",
                "add_get_local_id_get_work_dim",
                "add_get_group_id_get_work_dim",
                "add_get_local_size_get_work_dim",
                "add_get_global_id_get_work_dim",
                "add_get_global_size_get_work_dim",
                "add_get_num_groups_get_work_dim",
                "add_get_work_dim_get_work_dim"};

            cl_int error;

            auto program = test::getProgram(
                "test/resources/kernels/addition/addition.bin");
            const auto programInitRefCount = program->referenceCount;

            cl_uint kernelNum;
            error = clCreateKernelsInProgram(program, 0, nullptr, &kernelNum);

            CHECK(error == CL_SUCCESS);
            CHECK(kernelNum == expected.size());

            auto actual = std::vector<cl_kernel>(kernelNum);
            error = clCreateKernelsInProgram(program, kernelNum, actual.data(),
                                             nullptr);

            CHECK(error == CL_SUCCESS);

            const auto kernelToName = [](cl_kernel kernel) {
                return kernel->name;
            };
            CHECK(utils::joinToString<cl_kernel>(actual, "\n", kernelToName) ==
                  utils::joinToString(expected, "\n"));

            for (auto kernel : actual) {
                CHECK(kernel->program != nullptr);
                CHECK(kernel->referenceCount == 1);
            }
            CHECK(actual[0]->program->referenceCount ==
                  programInitRefCount + actual.size());
        }
    }

    TEST_CASE("clSetKernelArg") {
        SUBCASE("can set mem object argument") {
            const auto kernel = test::getKernel(binaryPath, kernelName);

            const auto size = 16;
            const auto sizeBytes = size * sizeof(cl_uint);
            auto vector = std::vector<cl_uint>{};
            test::fillVector(size, vector);

            const auto buffer =
                test::createBuffer(CL_MEM_USE_HOST_PTR, sizeBytes, &vector);

            const auto error = clSetKernelArg(kernel, 0, sizeBytes, &buffer);

            CHECK(error == CL_SUCCESS);
        }

        SUBCASE("sets null kernel argument") {
            const auto kernel = test::getKernel(binaryPath, kernelName);

            const auto error = clSetKernelArg(kernel, 0, 0, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(kernel->getArgument(0).value.has_value());
        }

        SUBCASE("fails if index is out of bounds") {
            const auto kernel = test::getKernel(binaryPath, kernelName);
            const auto argCount = kernel->argumentCount();

            const auto error = clSetKernelArg(kernel, argCount, 0, nullptr);

            CHECK(error == CL_INVALID_ARG_INDEX);
        }
    }

    TEST_CASE("clRetainKernel") {
        SUBCASE("increments kernel ref count") {
            const auto kernel = test::getKernel(binaryPath, kernelName);
            const auto initRefCount = kernel->referenceCount;

            const auto error = clRetainKernel(kernel);
            CHECK(error == CL_SUCCESS);
            CHECK(kernel->referenceCount == initRefCount + 1);
        }
    }

    TEST_CASE("clReleaseKernel") {
        SUBCASE("decrements kernel ref count") {
            const auto kernel = test::getKernel(binaryPath, kernelName);

            clRetainKernel(kernel);
            const auto initRefCount = kernel->referenceCount;

            const auto error = clReleaseKernel(kernel);

            CHECK(error == CL_SUCCESS);
            CHECK(kernel->referenceCount == initRefCount - 1);
        }
    }

    TEST_CASE("clEnqueueNDRangeKernel") {
        SUBCASE("executes kernel") {
            cl_int error;
            const auto queue = test::getCommandQueue();
            const auto kernel = test::getKernel(binaryPath, kernelName);

            const size_t arraySize = 3;
            const size_t arraySizeBytes = arraySize * sizeof(cl_uint);

            std::vector<cl_uint> data1{};
            data1.assign(arraySize, 1);
            const auto mem1 =
                test::createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   arraySizeBytes, data1.data());

            std::vector<cl_uint> data2{};
            data2.assign(arraySize, 2);
            const auto mem2 =
                test::createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   arraySizeBytes, data2.data());

            const auto mem3 =
                test::createBuffer(CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                   arraySizeBytes, data2.data());

            error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem1);
            CHECK(error == CL_SUCCESS);

            error = clSetKernelArg(kernel, 1, sizeof(cl_mem), &mem2);
            CHECK(error == CL_SUCCESS);

            error = clSetKernelArg(kernel, 2, sizeof(cl_mem), &mem3);
            CHECK(error == CL_SUCCESS);

            size_t globalWorkSize[1];
            globalWorkSize[0] = arraySizeBytes;

            size_t localWorkSize[1];
            localWorkSize[0] = 0;

            error = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr,
                                           globalWorkSize, localWorkSize, 0,
                                           nullptr, nullptr);
            CHECK(error == CL_SUCCESS);

            error = clFlush(queue);
            CHECK(error == CL_SUCCESS);

            std::vector<cl_uint> bufferData(arraySize);
            error = clEnqueueReadBuffer(queue, mem3, true, 0, arraySizeBytes,
                                        bufferData.data(), 0, nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(utils::joinToString<cl_uint>(bufferData, " ", [](auto value) {
                      return std::to_string(value);
                  }) == "0 0 0");
        }
    }

    TEST_CASE("clEnqueueTask") {
        SUBCASE("executes kernel") {
            cl_int error;
            const auto queue = test::getCommandQueue();
            const auto kernel = test::getKernel(binaryPath, kernelName);

            const size_t arraySize = 1;
            const size_t arraySizeBytes = arraySize * sizeof(cl_uint);

            std::vector<cl_uint> data1{};
            data1.assign(arraySize, 1);
            const auto mem1 =
                test::createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   arraySizeBytes, data1.data());

            std::vector<cl_uint> data2{};
            data2.assign(arraySize, 2);
            const auto mem2 =
                test::createBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                   arraySizeBytes, data2.data());

            const auto mem3 =
                test::createBuffer(CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
                                   arraySizeBytes, data2.data());

            error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem1);
            CHECK(error == CL_SUCCESS);

            error = clSetKernelArg(kernel, 1, sizeof(cl_mem), &mem2);
            CHECK(error == CL_SUCCESS);

            error = clSetKernelArg(kernel, 2, sizeof(cl_mem), &mem3);
            CHECK(error == CL_SUCCESS);

            error = clEnqueueTask(queue, kernel, 0, nullptr, nullptr);
            CHECK(error == CL_SUCCESS);

            error = clFlush(queue);
            CHECK(error == CL_SUCCESS);

            std::vector<cl_uint> bufferData(arraySize);
            error = clEnqueueReadBuffer(queue, mem3, true, 0, arraySizeBytes,
                                        bufferData.data(), 0, nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(utils::joinToString<cl_uint>(bufferData, " ", [](auto value) {
                      return std::to_string(value);
                  }) == "0");
        }
    }

    TEST_CASE("clGetKernelInfo") {
        SUBCASE("get kernel name") {
            auto kernel = test::getKernel(binaryPath, kernelName);

            char out[20];
            const auto error = clGetKernelInfo(kernel, CL_KERNEL_FUNCTION_NAME,
                                               20, out, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(std::string(out) == kernelName);
        }
    }

    TEST_CASE("clGetKernelArgInfo") {
        SUBCASE("get kernel arg type") {
            auto kernel = test::getKernel(binaryPath, kernelName);

            clSetKernelArg(kernel, 0, sizeof(cl_mem), nullptr);

            char out[20];
            const auto error = clGetKernelArgInfo(
                kernel, 0, CL_KERNEL_ARG_TYPE_NAME, 20, out, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(std::string(out) == "int*");
        }
    }
}
