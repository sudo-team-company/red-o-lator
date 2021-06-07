#pragma once

#include <memory>
#include <optional>
#include <utility>

#include "KernelArgumentInfo.h"
#include "KernelArgumentValue.hpp"

struct KernelArgument {
    explicit KernelArgument(std::shared_ptr<KernelArgumentInfo> info)
        : info(std::move(info)) {}

    const std::shared_ptr<KernelArgumentInfo> info;
    std::optional<KernelArgumentValue> value{};
};
