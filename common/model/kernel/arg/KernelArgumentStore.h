#ifndef RED_O_LATOR_KERNELARGUMENTSTORE_H
#define RED_O_LATOR_KERNELARGUMENTSTORE_H

#include <optional>
#include <vector>
#include "KernelArgumentKind.h"

struct KernelArgumentStore {
    std::string name;
    KernelArgumentKind kind;
    size_t size;
    bool readPermission;
    bool writePermission;
    std::optional<std::vector<uint8_t>> data;
    std::optional<std::vector<uint8_t>> output;
};

#endif  // RED_O_LATOR_KERNELARGUMENTSTORE_H
