#pragma once

#include <variant>

using CLObjectInfoParameterValueType = std::variant<void*, std::string>;

struct CLObjectInfoParameterValue {
    CLObjectInfoParameterValue(CLObjectInfoParameterValueType value,
                               size_t size)
        : value(std::move(value)), size(size) {}

    CLObjectInfoParameterValueType value;
    size_t size;
};
