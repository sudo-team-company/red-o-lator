#include <common/common.hpp>
#include <cstddef>
#include <string>
#include <utility>

#include "CLKernel.h"
#include "runtime-commons.h"

CLKernel::CLKernel(IcdDispatchTable* const dispatchTable,
                   std::string name,
                   std::vector<std::string> config,
                   std::vector<std::string> instructions,
                   std::vector<KernelArgument> arguments)
    : dispatchTable(dispatchTable),
      name(std::move(name)),
      config(std::move(config)),
      instructions(std::move(instructions)),
      arguments(std::move(arguments)) {}

void CLKernel::setArgument(cl_uint index, size_t size, const void* value) {
    if (index > arguments.size()) {
        throw KernelArgumentOutOfBoundsError(
            "Attempt to set argument at index " + std::to_string(index) +
            " of kernel with " + std::to_string(arguments.size()) +
            "arguments");
    }

    const auto copy = new std::byte[size];
    memcpy(copy, value, size);

    arguments[index].value = KernelArgumentValue(index, size, copy);
}

KernelArgument CLKernel::getArgument(cl_uint index) {
    if (index > arguments.size()) {
        throw KernelArgumentOutOfBoundsError(
            "Attempt to get argument at index " + std::to_string(index) +
            " of kernel with " + std::to_string(arguments.size()) +
            "arguments");
    }

    return arguments[index];
}
int CLKernel::argumentCount() {
    return arguments.size();
}

CLKernel* CLKernelBuilder::build() const {
    std::vector<KernelArgument> args;
    std::transform(argumentInfo.begin(), argumentInfo.end(),
                   std::back_inserter(args), [](auto& info) {
                       return KernelArgument(info);
                   });

    return new CLKernel(kDispatchTable, name, config, instructions, args);
}
