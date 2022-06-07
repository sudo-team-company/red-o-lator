#ifndef RED_O_LATOR_KERNELPROTO_H
#define RED_O_LATOR_KERNELPROTO_H

#include <string>
#include <vector>

struct ConfigProto {
    uint32_t dims;
    std::vector<size_t> globalWorkOffset;
    std::vector<size_t> globalWorkSize;
    std::vector<size_t> localWorkSize;
    std::vector<std::string> parameters;
};

struct ArgumentProto {
    enum Kind {
        POINTER,
        SCALAR
    };
    std::string name;
    Kind kind;
    std::string data;
    uint32_t size;
    bool readPermission;
    bool writePermission;
};

struct KernelProto {
    std::string name;
    ConfigProto config;
    std::vector<std::string> instructions;
    std::vector<ArgumentProto> args;
};

#endif  // RED_O_LATOR_KERNELPROTO_H
