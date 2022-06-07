#ifndef RED_O_LATOR_PARAMETER_H
#define RED_O_LATOR_PARAMETER_H

#include <string>

struct Parameter {
    std::string name;
    std::string value;

    explicit Parameter(std::string name, std::string value)
        : name{std::move(name)}, value{std::move(value)} {}
};

enum class ParametersCategory {
    GLOBAL, KERNEL
};

#endif  // RED_O_LATOR_PARAMETER_H
