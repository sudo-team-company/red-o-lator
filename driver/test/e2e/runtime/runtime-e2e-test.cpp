#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <common/test/doctest.h>
#include <CL/opencl.h>

std::vector<unsigned char> readBinaryFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),
                                      {});
}

TEST_CASE("a_plus_b") {
    cl_int error;

    cl_uint platformCount;
    error = clGetPlatformIDs(0, nullptr, &platformCount);
    CHECK(error == CL_SUCCESS);

    auto* platformList =
        (cl_platform_id*) malloc(platformCount * sizeof(cl_platform_id));

    error = clGetPlatformIDs(platformCount, platformList, nullptr);
    CHECK(error == CL_SUCCESS);

    const auto platform = platformList[0];

    for (int i = 0; i < platformCount; i++) {
        const auto currentPlatform = platformList[i];
        char platformName[128];
        error = clGetPlatformInfo(currentPlatform, CL_PLATFORM_NAME, 128,
                                      &platformName, nullptr);
        CHECK(error == CL_SUCCESS);
    }

    cl_uint num_devices;
    cl_device_id device;
    error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device,
                               &num_devices);
    CHECK(error == CL_SUCCESS);

    char deviceName[128];
    error =
        clGetDeviceInfo(device, CL_DEVICE_NAME, 128, deviceName, nullptr);
    CHECK(error == CL_SUCCESS);

    char deviceVersion[50];
    error =
        clGetDeviceInfo(device, CL_DEVICE_VERSION, 50, deviceVersion, nullptr);
    CHECK(error == CL_SUCCESS);

    cl_context context =
        clCreateContext(nullptr, 1, &device, nullptr, nullptr, &error);
    CHECK(error == CL_SUCCESS);

    cl_command_queue commandQueue =
        clCreateCommandQueue(context, device, 0, &error);
    CHECK(error == CL_SUCCESS);

    const size_t n = 1000;
    const size_t arraySize = n * sizeof(cl_uint);

    cl_mem mem1 = clCreateBuffer(context, CL_MEM_READ_ONLY, arraySize, nullptr,
                                 &error);
    CHECK(error == CL_SUCCESS);

    cl_mem mem2 = clCreateBuffer(context, CL_MEM_READ_ONLY, arraySize, nullptr,
                                 &error);
    CHECK(error == CL_SUCCESS);

    cl_mem mem3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, arraySize, nullptr,
                                 &error);
    CHECK(error == CL_SUCCESS);

    const std::string binaryPath =
        "/home/newuserkk/Projects/ITMO/thesis/red-o-lator/driver/test/"
        "resources/kernels/weighted_sum_kernel/weighted_sum_kernel.bin";
    const auto binary = readBinaryFile(binaryPath);

    CHECK_FALSE(binary.empty());

    const size_t binarySize[1] = {binary.size()};
    const unsigned char* binaryData[1] = {binary.data()};

    cl_program program = clCreateProgramWithBinary(
        context, 1, &device, binarySize, binaryData, nullptr, &error);
    CHECK(error == CL_SUCCESS);

    error = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    CHECK(error == CL_SUCCESS);

    const std::string kernelName = "weighted_sum_kernel";
    cl_kernel kernel = clCreateKernel(program, kernelName.c_str(), &error);
    CHECK(error == CL_SUCCESS);

    //    clEnqueueWriteBuffer(queue, mem1, false, 0, array_mem_sz, a, 0, 0, 0);
    //    clEnqueueWriteBuffer(queue, mem2, false, 0, array_mem_sz, b, 0, 0, 0);

    clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem1);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &mem2);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &mem3);

    //    size_t globalWorkOffset = 0;
    //    size_t globalWorkSize = 0;
    //    size_t localWorkSize = 0;
    //    error = clEnqueueNDRangeKernel(commandQueue, kernel, 1,
    //                                       &globalWorkOffset, &globalWorkSize,
    //                                       &localWorkSize, 0, nullptr,
    //                                       nullptr);

    // TODO(executeKernel): memory release stuff

}
