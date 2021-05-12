#include <CL/opencl.h>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

#include "KernelLoader.h"

#define CHECK_ERROR(message)                                                \
    if (errorCode != CL_SUCCESS) {                                          \
        throw KernelLoadException(std::string(message) + "\nError code: " + \
                                  std::to_string(errorCode));               \
    }

std::vector<unsigned char> readBinaryFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),
                                      {});
}

void KernelLoader::executeKernel(const std::string& kernelPath) {
    cl_int errorCode;

    cl_uint platformCount;
    errorCode = clGetPlatformIDs(0, nullptr, &platformCount);
    CHECK_ERROR("Failed to load platform count")

    std::cout << "KernelLoader::executeKernel --- "
              << "found " << platformCount << " platforms" << std::endl;

    cl_platform_id platformList[platformCount];
    errorCode = clGetPlatformIDs(platformCount, platformList, nullptr);
    CHECK_ERROR("Failed to load platform list")

    const auto platform = platformList[0];

    for (int i = 0; i < platformCount; i++) {
        const auto currentPlatform = platformList[i];
        char platformName[128];
        errorCode = clGetPlatformInfo(currentPlatform, CL_PLATFORM_NAME, 128,
                                      &platformName, nullptr);
        CHECK_ERROR("Failed to get platform name")
        std::cout << "KernelLoader::executeKernel --- "
                  << "found platform: " << platformName << std::endl;
    }

    cl_uint num_devices;
    cl_device_id device;
    errorCode = clGetDeviceIDs(platform, CL_DEVICE_TYPE_DEFAULT, 1, &device,
                               &num_devices);
    CHECK_ERROR("Failed to load device")

    char deviceName[128];
    errorCode =
        clGetDeviceInfo(device, CL_DEVICE_NAME, 128, deviceName, nullptr);
    CHECK_ERROR("Failed to get device name")

    // TODO(executeKernel): logger?
    std::cout << "KernelLoader::executeKernel --- "
              << "using device '" << deviceName << "'" << std::endl;

    cl_context context =
        clCreateContext(nullptr, 1, &device, nullptr, nullptr, &errorCode);
    CHECK_ERROR("Failed to create context")

    cl_command_queue commandQueue = clCreateCommandQueue(
        context, device, CL_QUEUE_PROFILING_ENABLE, &errorCode);
    CHECK_ERROR("Failed to create command queue")

    const size_t n = 1000;
    const size_t arraySize = n * sizeof(cl_uint);


    cl_mem mem1 = clCreateBuffer(context, CL_MEM_READ_ONLY, arraySize, nullptr,
                                 &errorCode);
    CHECK_ERROR("Error creating buffer 1");

    std::vector<cl_uint> vec1{};
    for (int i = 0; i < n; ++i) {
        vec1.push_back(i);
    }
    cl_uint* in1 = vec1.data();
    cl_uint out1[20];
    errorCode = clEnqueueWriteBuffer(commandQueue, mem1, CL_TRUE, 0, arraySize,
                                     in1, 0, nullptr, nullptr);
    CHECK_ERROR("Error write for buffer 1")

    errorCode = clEnqueueWriteBuffer(
        commandQueue, mem1, CL_TRUE, 70 * sizeof(cl_uint), 3 * sizeof(cl_uint),
        std::vector<cl_uint>({100000, 100001, 100002}).data(), 0, nullptr,
        nullptr);
    CHECK_ERROR("Error write 2 for buffer 1")

    errorCode =
        clEnqueueReadBuffer(commandQueue, mem1, CL_TRUE, 64 * sizeof(cl_uint),
                            20 * sizeof(cl_uint), out1, 0, nullptr, nullptr);
    CHECK_ERROR("Error read for buffer 1")

    std::vector<cl_uint> vec1Out(std::begin(out1), std::end(out1));
    for (auto item : vec1Out) {
        std::cout << std::to_string(item) << " ";
    }
    std::cout << std::endl;


    cl_mem mem2 = clCreateBuffer(context, CL_MEM_READ_ONLY, arraySize, nullptr,
                                 &errorCode);
    CHECK_ERROR("Error creating buffer 2");
    cl_mem mem3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, arraySize, nullptr,
                                 &errorCode);
    CHECK_ERROR("Error creating buffer 3");

    const std::string binaryPath =
        "/home/newuserkk/Projects/ITMO/thesis/red-o-lator/driver/test/disasm/"
        "linear_kernels/addition/addition.bin";
    const auto binary = readBinaryFile(binaryPath);

    if (binary.empty()) {
        throw KernelLoadException("Could not load file " + binaryPath);
    }

    const size_t binarySize[1] = {binary.size()};
    const unsigned char* binaryData[1] = {binary.data()};

    cl_program program = clCreateProgramWithBinary(
        context, 1, &device, binarySize, binaryData, nullptr, &errorCode);
    CHECK_ERROR("Failed to create program")

    errorCode = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    CHECK_ERROR("Failed to build program")

    //    clEnqueueWriteBuffer(queue, mem1, false, 0, array_mem_sz, a, 0, 0, 0);
    //    clEnqueueWriteBuffer(queue, mem2, false, 0, array_mem_sz, b, 0, 0, 0);
    //
    //        clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem1);
    //    clSetKernelArg(kernel, 1, sizeof(cl_mem), &mem2);
    //    clSetKernelArg(kernel, 2, sizeof(cl_mem), &mem3);

    const std::string kernelName = "kernelName";
    cl_kernel kernel = clCreateKernel(program, kernelName.c_str(), &errorCode);
    CHECK_ERROR("Failed to create kernel with name " + kernelName)

    // TODO(executeKernel): create buffers and memory, set kernel args

    //    size_t globalWorkOffset = 0;
    //    size_t globalWorkSize = 0;
    //    size_t localWorkSize = 0;
    //    errorCode = clEnqueueNDRangeKernel(commandQueue, kernel, 1,
    //                                       &globalWorkOffset, &globalWorkSize,
    //                                       &localWorkSize, 0, nullptr,
    //                                       nullptr);

    // TODO(executeKernel): memory release stuff
}
