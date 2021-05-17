#pragma once

#include <common/test/doctest.h>

#include "runtime/icd/CLPlatformId.hpp"
#include "runtime/icd/CLDeviceId.hpp"
#include "runtime/icd/CLContext.h"
#include "runtime/icd/CLCommandQueue.h"

namespace test {
struct DummyCommand : public Command {
    void execute() const override{};
};

cl_platform_id getPlatform();
cl_device_id getDevice();
cl_context getContext();
cl_command_queue getCommandQueue();
}  // namespace test
