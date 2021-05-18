#include <vector>

#include "test-commons.h"

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

    CHECK(error == CL_SUCCESS);
    CHECK(queue != nullptr);

    return queue;
}

std::pair<cl_mem, cl_int> createBufferSafe(cl_mem_flags flags,
                                           size_t size,
                                           void* hostPtr) {
    auto context = test::getContext();
    const auto initRefCount = context->referenceCount;
    cl_int errorCode;
    cl_mem buffer = clCreateBuffer(context, flags, size, hostPtr, &errorCode);

    if (errorCode != CL_SUCCESS) {
        CHECK(context->referenceCount == initRefCount);
    }

    return std::make_pair(buffer, errorCode);
}

cl_mem createBuffer(cl_mem_flags flags, size_t size, void* hostPtr) {
    const auto [buffer, error] = createBufferSafe(flags, size, hostPtr);

    CHECK(error == CL_SUCCESS);
    CHECK(buffer != nullptr);

    return buffer;
}
}  // namespace test
