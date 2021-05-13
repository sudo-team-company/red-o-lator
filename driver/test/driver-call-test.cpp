#include <iostream>
#include "runtime/icd/icd.h"

int main() {
    cl_uint num_platforms;
    CLPlatformId* platforms;
    cl_int error = clGetPlatformIDs(1, &platforms, &num_platforms);
    std::cout << "error: " << error << std::endl;
    std::cout << "platforms: " << platforms->vendor << std::endl;
    std::cout << "num_platforms: " << num_platforms << std::endl;

    return 0;
}
