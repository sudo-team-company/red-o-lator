#pragma once

struct KernelArgumentValue {
    KernelArgumentValue(cl_uint index, size_t size, void* value)
        : index(index), size(size), value(value) {}

    cl_uint index;
    size_t size;
    void* value;
};
