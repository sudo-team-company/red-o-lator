#pragma once

#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>

#include "icd/icd.h"
#include "CLObjectInfoParameterValue.hpp"

class DeviceConfigurationParser {
   public:
    void load(const std::string& configurationFilePath);

    std::optional<CLObjectInfoParameterValue> getParameter(
        cl_device_info parameter) const;

    template <class T>
    T requireParameter(cl_device_info parameter) const;

   private:
    struct ParsedParameter {
        ParsedParameter(cl_device_info name, CLObjectInfoParameterValue value)
            : name(name), value(std::move(value)) {}

        cl_device_info name;
        CLObjectInfoParameterValue value;
    };

    std::string mConfigurationPath;
    std::unordered_map<cl_device_info, CLObjectInfoParameterValue>
        mParameters;

    static ParsedParameter parseParameter(const std::string& parameterName,
                                          const std::string& value);
};

struct DeviceConfigurationParseException : public std::runtime_error {
    explicit DeviceConfigurationParseException(const char* message)
        : std::runtime_error(message){};

    explicit DeviceConfigurationParseException(const std::string& message)
        : std::runtime_error(message){};
};

#include "DeviceConfigurationParser.tpp"
