#pragma once

#include <string>
#include <variant>

struct CLObjectInfoParameterValueTypeArray {
    explicit CLObjectInfoParameterValueTypeArray(void* array) : array(array) {}

    const void* array;
};

using CLObjectInfoParameterValueType =
    std::variant<void*, std::string, CLObjectInfoParameterValueTypeArray>;

struct CLObjectInfoParameterValue {
    CLObjectInfoParameterValue(CLObjectInfoParameterValueType value,
                               size_t size)
        : value(std::move(value)), size(size) {}

    CLObjectInfoParameterValueType value;
    size_t size;
};
