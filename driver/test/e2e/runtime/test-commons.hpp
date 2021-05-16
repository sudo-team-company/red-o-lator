#pragma once

cl_uint getPlatformCount() {
    cl_uint platformCount = 0;
    const auto errorCode = clGetPlatformIDs(0, nullptr, &platformCount);
    return platformCount;
}
