#pragma once

#include "flow/wavefront.h"

struct Dispatcher {
public:
    Dispatcher(const KernelConfig *config, const KernelCode *code);

    bool has_next_wg() const;

    std::unique_ptr<WorkGroup> next_wg();

private:
    size_t curIdX = 0;
    size_t curIdY = 0;
    size_t curIdZ = 0;
    size_t dispatchedWg = 0;
    size_t wgAmount;
    const KernelConfig *const kernelConfig;
    const KernelCode *code;
};