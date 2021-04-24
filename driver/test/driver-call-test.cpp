#include <iostream>
#include "opencl.h"

int main() {
    cl_uint num_platforms;
    cl_int error = clGetPlatformIDs(0, nullptr, &num_platforms);

    std::cout << "error: " << error << std::endl;
}
