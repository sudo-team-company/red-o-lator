#pragma once

#include <common/test/doctest.h>

#include "runtime/icd/CLPlatformId.hpp"
#include "runtime/icd/CLDeviceId.hpp"
#include "runtime/icd/CLContext.h"

namespace test {
cl_platform_id getPlatform();
cl_device_id getDevice();
cl_context getContext();
}  // namespace test
