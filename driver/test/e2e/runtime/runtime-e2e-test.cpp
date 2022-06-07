#include <iostream>
#include <string>
#include <vector>

#include <CL/opencl.h>
#include <common/test/doctest.h>
#include <common/utils/common.hpp>

TEST_CASE("a_plus_b") {
    cl_int error;

    cl_uint platformCount;
    error = clGetPlatformIDs(0, nullptr, &platformCount);
    REQUIRE(error == CL_SUCCESS);

    auto* platformList =
        (cl_platform_id*) malloc(platformCount * sizeof(cl_platform_id));

    error = clGetPlatformIDs(platformCount, platformList, nullptr);
    REQUIRE(error == CL_SUCCESS);

    cl_platform_id platform;

    for (int i = 0; i < platformCount; i++) {
        cl_platform_id currentPlatform = platformList[i];
        char platformVendor[128];
        error = clGetPlatformInfo(currentPlatform, CL_PLATFORM_VENDOR, 128,
                                  &platformVendor, nullptr);
        REQUIRE(error == CL_SUCCESS);

        if (std::string{platformVendor} == "sudo-team-company") {
            platform = currentPlatform;
        }
    }

    cl_uint num_devices;
    cl_device_id device;
    error =
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
    REQUIRE(error == CL_SUCCESS);

    char deviceName[128];
    error = clGetDeviceInfo(device, CL_DEVICE_NAME, 128, deviceName, nullptr);
    REQUIRE(error == CL_SUCCESS);

    char deviceVersion[50];
    error =
        clGetDeviceInfo(device, CL_DEVICE_VERSION, 50, deviceVersion, nullptr);
    REQUIRE(error == CL_SUCCESS);

    cl_context context =
        clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);
    REQUIRE(error == CL_SUCCESS);

    cl_command_queue queue = clCreateCommandQueue(
        context, device, CL_QUEUE_PROFILING_ENABLE, &error);
    REQUIRE(error == CL_SUCCESS);

    const std::string binaryPath = "test/resources/kernels/a_plus_b.bin";
    const auto binary = utils::readBinaryFile(binaryPath);

    REQUIRE_FALSE(binary.empty());

    const size_t binarySize[1] = {binary.size()};
    const unsigned char* binaryData[1] = {binary.data()};

    cl_program program = clCreateProgramWithBinary(
        context, 1, &device, binarySize, binaryData, nullptr, &error);
    REQUIRE(error == CL_SUCCESS);

    error = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    REQUIRE(error == CL_SUCCESS);

    const std::string kernelName = "a_plus_b";
    cl_kernel kernel = clCreateKernel(program, kernelName.c_str(), &error);
    REQUIRE(error == CL_SUCCESS);

    const size_t arraySize = 3;
    const size_t arraySizeBytes = arraySize * sizeof(cl_uint);

    std::vector<cl_uint> data1{};
    data1.assign(arraySize, 1);
    cl_mem mem1 =
        clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                       arraySizeBytes, data1.data(), &error);
    REQUIRE(error == CL_SUCCESS);

    std::vector<cl_uint> data2{};
    data2.assign(arraySize, 2);
    cl_mem mem2 =
        clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                       arraySizeBytes, data2.data(), &error);
    REQUIRE(error == CL_SUCCESS);

    cl_mem mem3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, arraySizeBytes,
                                 nullptr, &error);
    REQUIRE(error == CL_SUCCESS);

    error = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem1);
    REQUIRE(error == CL_SUCCESS);

    error = clSetKernelArg(kernel, 1, sizeof(cl_mem), &mem2);
    REQUIRE(error == CL_SUCCESS);

    error = clSetKernelArg(kernel, 2, sizeof(cl_mem), &mem3);
    REQUIRE(error == CL_SUCCESS);

    size_t globalWorkSize[1];
    globalWorkSize[0] = arraySizeBytes;

    size_t localWorkSize[1];
    localWorkSize[0] = 0;

    cl_event event;
    error = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, globalWorkSize,
                                   localWorkSize, 0, nullptr, &event);
    REQUIRE(error == CL_SUCCESS);

    error = clFlush(queue);
    REQUIRE(error == CL_SUCCESS);

    cl_int eventStatus = 1;
    error = clGetEventInfo(event, CL_EVENT_COMMAND_EXECUTION_STATUS,
                           sizeof(cl_int), &eventStatus, nullptr);
    REQUIRE(error == CL_SUCCESS);
    REQUIRE(eventStatus == CL_COMPLETE);

    cl_ulong timeStart = 0;
    cl_ulong timeEnd = 0;
    error = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START,
                                    sizeof(cl_ulong), &timeStart, nullptr);
    REQUIRE(error == CL_SUCCESS);
    REQUIRE(timeStart > 0);

    error = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END,
                                    sizeof(cl_ulong), &timeEnd, nullptr);
    REQUIRE(error == CL_SUCCESS);
    REQUIRE(timeEnd > 0);
    REQUIRE(timeEnd - timeStart > 0);

    std::vector<cl_uint> bufferData(arraySize);
    error = clEnqueueReadBuffer(queue, mem3, true, 0, arraySizeBytes,
                                bufferData.data(), 0, nullptr, nullptr);
    REQUIRE(error == CL_SUCCESS);
    REQUIRE(utils::joinToString<cl_uint>(bufferData, " ", [](auto value) {
                return std::to_string(value);
            }) == "0 0 0");

    error = clReleaseKernel(kernel);
    REQUIRE(error == CL_SUCCESS);

    error = clReleaseMemObject(mem3);
    REQUIRE(error == CL_SUCCESS);

    error = clReleaseMemObject(mem2);
    REQUIRE(error == CL_SUCCESS);

    error = clReleaseMemObject(mem1);
    REQUIRE(error == CL_SUCCESS);

    error = clReleaseProgram(program);
    REQUIRE(error == CL_SUCCESS);

    error = clReleaseCommandQueue(queue);
    REQUIRE(error == CL_SUCCESS);

    error = clReleaseContext(context);
    REQUIRE(error == CL_SUCCESS);
}
