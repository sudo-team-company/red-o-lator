#include "KernelArgumentInfo.h"

std::set<std::string> cartesianProduct(const std::set<std::string>& first,
                                       const std::set<std::string>& second) {
    std::set<std::string> result;

    for (const auto& s1 : first) {
        for (const auto& s2 : second) {
            result.emplace(s1 + s2);
        }
    }

    return result;
}

const std::set<std::string>
    ScalarKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{
        "char",   "uchar",     "short",    "ushort",    "int",  "uint",
        "ulong",  "long",      "float",    "double",    "bool", "half",
        "size_t", "ptrdiff_t", "intptr_t", "uintptr_t", "void"};

const std::set<std::string> VectorKernelArgumentInfo::
    typeNamePrefixVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"char", "uchar", "short", "ushort", "int",
                          "uint", "ulong", "long",  "float",  "double"};

const std::set<std::string> VectorKernelArgumentInfo::
    typeNameSuffixVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"2", "3", "4", "8", "16"};

const std::set<std::string>
    VectorKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    cartesianProduct(typeNamePrefixVariants, typeNameSuffixVariants);

const std::set<std::string>
    StructureKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"structure"};

const std::set<std::string>
    ImageKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"image1d_t", "image1d_array_t",
                          "image1d_buffer_t", "image2d_t", "image2d_array_t",
                          "image3d_t"};

const std::set<std::string>
    SamplerKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"sampler_t"};

const std::set<std::string>
    EventKernelArgumentInfo::typeNameVariants =  // NOLINT(cert-err58-cpp)
    std::set<std::string>{"event_t"};
