#pragma once

#include <optional>
#include <set>
#include <string>

#include "model/kernel/arg/KernelArgumentKind.h"
#include "runtime/icd/icd.h"

struct KernelArgumentInfo {
    virtual ~KernelArgumentInfo() = default;

    KernelArgumentKind kind;
    cl_uint index;
    std::string typeName;
    std::string type;
    std::string argumentName;
    bool unused = false;
};

struct ScalarKernelArgumentInfo : public KernelArgumentInfo {
    static const std::set<std::string> typeNameVariants;
};

struct VectorKernelArgumentInfo : public KernelArgumentInfo {
   public:
    static const std::set<std::string> typeNameVariants;

   private:
    static const std::set<std::string> typeNamePrefixVariants;
    static const std::set<std::string> typeNameSuffixVariants;
};

struct StructureKernelArgumentInfo : public KernelArgumentInfo {
    size_t structSize = 0;

    static const std::set<std::string> typeNameVariants;
};

struct ImageKernelArgumentInfo : public KernelArgumentInfo {
    cl_kernel_arg_access_qualifier accessQualifier =
        CL_KERNEL_ARG_ACCESS_READ_WRITE;
    cl_uint resId = 0;

    static const std::set<std::string> typeNameVariants;
};

struct SamplerKernelArgumentInfo : public KernelArgumentInfo {
    cl_uint resId = 0;

    static const std::set<std::string> typeNameVariants;
};

struct EventKernelArgumentInfo : public KernelArgumentInfo {
    static const std::set<std::string> typeNameVariants;
};

struct PointerKernelArgumentInfo : public KernelArgumentInfo {
    cl_kernel_arg_address_qualifier addressQualifier =
        CL_KERNEL_ARG_ADDRESS_GLOBAL;
    cl_kernel_arg_access_qualifier accessQualifier =
        CL_KERNEL_ARG_ACCESS_READ_WRITE;
    cl_kernel_arg_type_qualifier argTypeQualifier = CL_KERNEL_ARG_TYPE_NONE;
    size_t structSize = 0;
    size_t constSize = 0;
};
