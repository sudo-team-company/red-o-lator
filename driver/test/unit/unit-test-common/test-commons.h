#pragma once

#include <common/test/doctest.h>

#include "runtime/icd/CLCommandQueue.h"
#include "runtime/icd/CLContext.h"
#include "runtime/icd/CLDeviceId.hpp"
#include "runtime/icd/CLPlatformId.hpp"

namespace test {
struct DummyCommand : public Command {
    void execute() const override{};
};

cl_platform_id getPlatform();
cl_device_id getDevice();
cl_context getContext();
cl_command_queue getCommandQueue();

std::pair<cl_mem, cl_int> createBufferSafe(cl_mem_flags flags = 0,
                                           size_t size = 128,
                                           void* hostPtr = nullptr);

cl_mem createBuffer(cl_mem_flags flags = 0,
                    size_t size = 128,
                    void* hostPtr = nullptr);
}  // namespace test
