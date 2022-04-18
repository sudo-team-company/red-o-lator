#pragma once

#include <stdexcept>
#include <string>

class KernelLoader {
   public:
    static void executeKernel(const std::string& kernelPath) ;
};

class KernelLoadException : public std::runtime_error {
   public:
    explicit KernelLoadException(const std::string& message)
        : runtime_error(message) {}
    explicit KernelLoadException(const char* message) : runtime_error(message) {}
};
