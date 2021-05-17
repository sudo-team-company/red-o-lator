#pragma once

#include <common/test/doctest.h>

#include "runtime/icd/CLPlatformId.hpp"
#include "runtime/icd/CLDeviceId.hpp"

namespace test {
cl_platform_id getPlatform();
cl_device_id getDevice();
}  // namespace test
