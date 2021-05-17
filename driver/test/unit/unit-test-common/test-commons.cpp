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
}  // namespace test
