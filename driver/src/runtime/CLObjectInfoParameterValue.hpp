#pragma once

#include <string>
#include <variant>

using CLObjectInfoParameterValueType = std::variant<void*, std::string>;

struct CLObjectInfoParameterValue {
    CLObjectInfoParameterValue(CLObjectInfoParameterValueType value,
                               size_t size, bool isArray = false)
        : value(std::move(value)), size(size), isArray(isArray) {}

    CLObjectInfoParameterValueType value;
    size_t size;

    // TODO: possible memory leak, may be resolved by template array wrapper in
    //   CLObjectInfoParameterValueType
    bool isArray = false;
};
