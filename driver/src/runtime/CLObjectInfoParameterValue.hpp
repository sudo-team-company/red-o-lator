#pragma once

#include <string>
#include <variant>

using CLObjectInfoParameterValueType = std::variant<void*, std::string>;

struct CLObjectInfoParameterValue {
    CLObjectInfoParameterValue(CLObjectInfoParameterValueType value,
                               size_t size, bool isPointer = false)
        : value(std::move(value)), size(size), isPointer(isPointer) {}

    CLObjectInfoParameterValueType value;
    size_t size;

    // TODO: possible memory leak, may be resolved by template array wrapper in
    //   CLObjectInfoParameterValueType
    bool isPointer = false;
};
