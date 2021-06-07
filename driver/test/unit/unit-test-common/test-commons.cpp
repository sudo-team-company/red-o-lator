#include <common/utils/file-utlis.hpp>
#include <vector>

#include "test-commons.h"

#include <runtime/common/runtime-commons.h>

namespace test {
cl_platform_id getPlatform() {
    auto platformList = std::vector<cl_platform_id>(1);
    const auto errorCode = clGetPlatformIDs(1, platformList.data(), nullptr);

    REQUIRE(errorCode == CL_SUCCESS);

    return platformList[0];
}

cl_device_id getDevice() {
    cl_platform_id platform = getPlatform();
    auto deviceList = std::vector<cl_device_id>(1);
    const auto errorCode = clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1,
                                          deviceList.data(), nullptr);

    REQUIRE(errorCode == CL_SUCCESS);

    return deviceList[0];
}

cl_context getContext() {
    cl_int error;
    const std::vector<cl_device_id> devices = {test::getDevice()};
    cl_context context =
        clCreateContext(nullptr, 1, devices.data(), nullptr, nullptr, &error);
    REQUIRE(error == CL_SUCCESS);
    REQUIRE(context != nullptr);

    return context;
}

cl_command_queue getCommandQueue() {
    const auto context = test::getContext();
    cl_int error;
    const auto queue =
        clCreateCommandQueue(context, context->device, 0, &error);

    REQUIRE(error == CL_SUCCESS);
    REQUIRE(queue != nullptr);

    return queue;
}

Command* getDummyCommand(cl_command_queue queue) {
    const auto command = new DummyCommand(queue);
    const auto event = new CLEvent(kDispatchTable, queue->context, kClock, command);
    command->setEvent(event);
    return command;
}

std::pair<cl_mem, cl_int> createBufferSafe(cl_mem_flags flags,
                                           size_t size,
                                           void* hostPtr) {
    auto context = test::getContext();
    const auto initRefCount = context->referenceCount;
    cl_int errorCode;
    cl_mem buffer = clCreateBuffer(context, flags, size, hostPtr, &errorCode);

    if (errorCode != CL_SUCCESS) {
        REQUIRE(context->referenceCount == initRefCount);
    }

    return std::make_pair(buffer, errorCode);
}

cl_mem createBuffer(cl_mem_flags flags, size_t size, void* hostPtr) {
    const auto [buffer, error] = createBufferSafe(flags, size, hostPtr);

    REQUIRE(error == CL_SUCCESS);
    REQUIRE(buffer != nullptr);

    return buffer;
}

cl_program getProgram(const std::string& binaryPath) {
    const auto context = test::getContext();
    const cl_device_id device[1] = {context->device};

    const auto binary = utils::readBinaryFile(binaryPath);
    const size_t binarySize[1] = {binary.size()};
    const unsigned char* binaryData[1] = {binary.data()};

    cl_int binaryStatus;
    cl_int error;

    const auto program = clCreateProgramWithBinary(
        context, 1, device, binarySize, binaryData, &binaryStatus, &error);

    CHECK(error == CL_SUCCESS);
    CHECK(binaryStatus == CL_SUCCESS);
    CHECK(program != nullptr);

    return program;
}

cl_kernel getKernel(const std::string& binaryPath,
                    const std::string& kernelName) {
    auto program = test::getProgram(binaryPath);

    cl_int error;
    const auto kernel = clCreateKernel(program, kernelName.c_str(), &error);

    CHECK(error == CL_SUCCESS);
    CHECK(kernel != nullptr);

    return kernel;
}

void fillVector(int n, std::vector<cl_uint>& out) {
    for (int i = 0; i < n; ++i) {
        out.push_back(i);
    }
}
}  // namespace test
