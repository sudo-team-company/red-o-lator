#ifndef RED_O_LATOR_KERNEL_H
#define RED_O_LATOR_KERNEL_H

#include <string>
#include <utility>
#include <vector>
#include "KernelConfig.h"
#include "model/instr/instruction.h"
#include "model/kernel/arg/KernelArgumentStore.h"

struct Kernel {
    std::string name;
    KernelCode code;
    KernelConfig config;
    std::vector<KernelArgumentStore> arguments;

    explicit Kernel(std::string name,
                    KernelCode code,
                    KernelConfig config,
                    std::vector<KernelArgumentStore> arguments)
        : name(std::move(name)),
          code{std::move(code)},
          config{std::move(config)},
          arguments{std::move(arguments)} {};
};

#endif  // RED_O_LATOR_KERNEL_H
