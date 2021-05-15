#include "KernelArgumentInfo.h"

const std::set<std::string>
    ScalarKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"char", "uchar", "short", "ushort", "int",
                          "uint", "ulong", "long",  "float",  "double"};

const std::set<std::string>
    StructureKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"structure"};

const std::set<std::string>
    ImageKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"image",          "image1d", "image1d_array",
                          "image1d_buffer", "image2d", "image2d_array",
                          "image3d"};

const std::set<std::string>
    SamplerKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"sampler"};
