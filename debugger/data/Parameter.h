#ifndef RED_O_LATOR_PARAMETER_H
#define RED_O_LATOR_PARAMETER_H

#include <string>

struct Parameter {
    std::string name;
    std::string value;  // ok for read only view
};


enum class ParametersCategory {
    GLOBAL, KERNEL
};

#endif  // RED_O_LATOR_PARAMETER_H
