#pragma once

template <typename T>
T DeviceConfigurationParser::requireParameter(cl_device_info parameter) const {
    const auto maybeParameter = getParameter(parameter);
    if (maybeParameter.has_value()) {
        const auto value = maybeParameter.value().value;
        const void* nonCastedValue;
        if (std::holds_alternative<std::string>(value)) {
            nonCastedValue =
                reinterpret_cast<const void*>(&std::get<std::string>(value));
        } else if (std::holds_alternative<CLObjectInfoParameterValueTypeArray>(
                       value)) {
            nonCastedValue =
                std::get<CLObjectInfoParameterValueTypeArray>(value).array;
        } else {
            nonCastedValue = std::get<void*>(value);
        }

        return reinterpret_cast<T>(nonCastedValue);
    }
}
