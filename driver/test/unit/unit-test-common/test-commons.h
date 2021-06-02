#pragma once

#include <common/test/doctest.h>

#include "runtime/icd/CLCommandQueue.h"
#include "runtime/icd/CLContext.h"
#include "runtime/icd/CLDeviceId.hpp"
#include "runtime/icd/CLPlatformId.hpp"

namespace test {
struct DummyCommand : public Command {
    explicit DummyCommand(cl_command_queue commandQueue)
        : Command(commandQueue) {}

   protected:
    void executeImpl() const override{};

   public:
    cl_command_type getCommandType() override {
        return CL_COMMAND_USER;
    }
};

cl_platform_id getPlatform();
cl_device_id getDevice();
cl_context getContext();
cl_command_queue getCommandQueue();
Command* getDummyCommand(cl_command_queue queue);

std::pair<cl_mem, cl_int> createBufferSafe(cl_mem_flags flags = 0,
                                           size_t size = 128,
                                           void* hostPtr = nullptr);

cl_mem createBuffer(cl_mem_flags flags = 0,
                    size_t size = 128,
                    void* hostPtr = nullptr);

cl_program getProgram(const std::string& binaryPath);

cl_kernel getKernel(const std::string& binaryPath,
                    const std::string& kernelName);

void fillVector(int n, std::vector<cl_uint>& out);
}  // namespace test