#include "CLKernel.h"

#include <common/common.hpp>
#include <string>
#include <utility>

KernelArgument KernelArgument::UNSET =  // NOLINT(cert-err58-cpp)
    KernelArgument(0, 0, nullptr);

CLKernel::CLKernel(IcdDispatchTable* const dispatchTable,
                   std::string name,
                   cl_uint argumentCount,
                   std::vector<std::string> config,
                   std::vector<std::string> instructions)
    : dispatchTable(dispatchTable),
      name(std::move(name)),
      argumentCount(argumentCount),
      config(std::move(config)),
      instructions(std::move(instructions)) {
    arguments.assign(argumentCount, KernelArgument::UNSET);
}

void CLKernel::setArgument(cl_uint index, size_t size, void* value) {
    if (index > arguments.size()) {
        throw KernelArgumentOutOfBoundsError(
            "Attempt to set argument at index " + std::to_string(index) +
            " with maximum index of " + std::to_string(argumentCount - 1));
    }

    arguments[index] = KernelArgument(index, size, value);
}

std::optional<KernelArgument> CLKernel::getArgument(cl_uint index) {
    if (index > arguments.size()) {
        throw KernelArgumentOutOfBoundsError(
            "Attempt to get argument at index " + std::to_string(index) +
            " with maximum index of " + std::to_string(argumentCount - 1));
    }

    if (&arguments[index] == &KernelArgument::UNSET) {
        return utils::optionalOf<KernelArgument>();
    }

    return utils::optionalOf(arguments[index]);
}
