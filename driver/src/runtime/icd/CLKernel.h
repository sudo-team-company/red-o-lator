#pragma once

#include <ocl_icd.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "IcdDispatchTable.h"

struct KernelArgument {
    KernelArgument(cl_uint index, size_t size, void* value)
        : index(index), size(size), value(value) {}

    cl_uint index;
    size_t size;
    void* value;

    static KernelArgument UNSET;
};

class KernelArgumentOutOfBoundsError : public std::runtime_error {
   public:
    explicit KernelArgumentOutOfBoundsError(const std::string& message)
        : std::runtime_error(message){};
    explicit KernelArgumentOutOfBoundsError(const char* message)
        : std::runtime_error(message){};
};

class CLKernel {
   public:
    CLKernel(IcdDispatchTable* dispatchTable,
             std::string name,
             cl_uint argumentCount,
             std::vector<std::string> config,
             std::vector<std::string> instructions);

    void setArgument(cl_uint index, size_t size, void* value);

    std::optional<KernelArgument> getArgument(cl_uint index);

    IcdDispatchTable* const dispatchTable;
    const std::string name;
    const cl_uint argumentCount;
    const std::vector<std::string> config;
    const std::vector<std::string> instructions;

   private:
    std::vector<KernelArgument> arguments;
};
