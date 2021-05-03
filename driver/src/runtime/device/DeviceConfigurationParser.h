#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <optional>
#include <stdexcept>
#include <variant>

#include "runtime/icd.h"

using DeviceConfigurationParameterValueType = std::variant<void*, std::string>;

struct DeviceConfigurationParameterValue {
    DeviceConfigurationParameterValue(
        DeviceConfigurationParameterValueType value, size_t size)
        : value(std::move(value)), size(size) {}

    DeviceConfigurationParameterValueType value;
    size_t size;
};

class DeviceConfigurationParser {
   public:
    void load(const std::string& configurationFilePath);

    std::optional<DeviceConfigurationParameterValue> getParameter(
        cl_device_info parameter) const;

   private:
    struct ParsedParameter {
        ParsedParameter(cl_device_info name,
                        DeviceConfigurationParameterValue value)
            : name(name), value(std::move(value)) {}

        cl_device_info name;
        DeviceConfigurationParameterValue value;
    };

    std::string mConfigurationPath;
    std::unordered_map<cl_device_info, DeviceConfigurationParameterValue>
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
