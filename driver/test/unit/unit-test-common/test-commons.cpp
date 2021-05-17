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
    cl_context context = clCreateContext(nullptr, 1, devices.data(), nullptr,
                                         nullptr, &error);
    REQUIRE(error == CL_SUCCESS);
    REQUIRE(context != nullptr);

    return context;
}
}  // namespace test
