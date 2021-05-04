#include <CL/opencl.h>
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

    auto* platformList =
        (cl_platform_id*) malloc(platformCount * sizeof(platformCount));
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

    //    const size_t arraySize = 1000;
    //    cl_mem mem1 = clCreateBuffer(context, CL_MEM_READ_ONLY, arraySize,
    //    nullptr,
    //                                 &errorCode);
    //    CHECK_ERROR("Error creating buffer 1");
    //    cl_mem mem2 = clCreateBuffer(context, CL_MEM_READ_ONLY, arraySize,
    //    nullptr,
    //                                 &errorCode);
    //    CHECK_ERROR("Error creating buffer 2");
    //    cl_mem mem3 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, arraySize,
    //    nullptr,
    //                                 &errorCode);
    //    CHECK_ERROR("Error creating buffer 3");

    cl_command_queue commandQueue = clCreateCommandQueue(
        context, device, CL_QUEUE_PROFILING_ENABLE, &errorCode);
    CHECK_ERROR("Failed to create command queue")

    const auto binary = readBinaryFile(
        "../driver/test/disasm/linear_kernels/addition/addition.bin");
    const size_t binarySize[1] = {binary.size()};
    const unsigned char* binaryData[1] = {binary.data()};

    cl_program program = clCreateProgramWithBinary(
        context, 1, &device, binarySize, binaryData, nullptr, &errorCode);
    CHECK_ERROR("Failed to create program")

    errorCode = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    CHECK_ERROR("Failed to build program")

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
