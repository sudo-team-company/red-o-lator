#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <memory>
#include <vector>

#include "KernelArgument.hpp"
#include "runtime/icd/icd.h"

struct KernelWorkGroupSize {
    size_t x;
    size_t y;
    size_t z;
};

struct CLKernel {
   public:
    CLKernel(IcdDispatchTable* dispatchTable,
             std::string name,
             KernelWorkGroupSize requiredWorkGroupSize,
             std::vector<std::string> config,
             std::vector<std::string> instructions,
             std::vector<KernelArgument> arguments);

    ~CLKernel();

    void setArgument(cl_uint index, size_t size, const void* value);

    [[nodiscard]] KernelArgument getArgument(cl_uint index) const;

    [[nodiscard]] const std::vector<KernelArgument>& getArguments() const;

    [[nodiscard]] int argumentCount() const;

    IcdDispatchTable* const dispatchTable;
    const std::string name;
    const KernelWorkGroupSize requiredWorkGroupSize;
    const std::vector<std::string> config{};
    const std::vector<std::string> instructions{};

    CLProgram* program{};

    unsigned int referenceCount = 0;

   private:
    void releaseArgument(KernelArgument& argument);

    std::vector<KernelArgument> arguments;
};

struct CLKernelBuilder {
    [[nodiscard]] CLKernel* build() const;

    std::string name;

    KernelWorkGroupSize requiredWorkGroupSize{};

    std::vector<std::string> config{};
    std::vector<std::string> instructions{};
    std::vector<std::shared_ptr<KernelArgumentInfo>> argumentInfo{};
};

class KernelArgumentOutOfBoundsError : public std::runtime_error {
   public:
    explicit KernelArgumentOutOfBoundsError(const std::string& message)
        : std::runtime_error(message){};
    explicit KernelArgumentOutOfBoundsError(const char* message)
        : std::runtime_error(message){};
};
