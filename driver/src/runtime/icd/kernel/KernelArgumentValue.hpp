#pragma once

#include <utility>
#include <variant>
#include <runtime/icd/CLMem.h>

using KernelArgumentValueType = std::variant<void*, CLMem*>;

struct KernelArgumentValue {
    KernelArgumentValue(cl_uint index,
                        size_t size,
                        KernelArgumentValueType value)
        : index(index), size(size), value(std::move(value)) {}

    cl_uint index{};
    size_t size;
    KernelArgumentValueType value;
};
