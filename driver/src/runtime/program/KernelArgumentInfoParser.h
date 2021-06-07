#pragma once

#include <vector>
#include <memory>
#include <string>
#include <utility>

#include "runtime/kernel/KernelArgumentInfo.h"

class KernelArgumentInfoParser {
   public:
    explicit KernelArgumentInfoParser(int index, std::string asmLine)
        : argIndex(index), asmLine(std::move(asmLine)) {}

    std::shared_ptr<KernelArgumentInfo> parse();

   private:
    void parse(const std::shared_ptr<ScalarKernelArgumentInfo>& outInfo);
    void parse(const std::shared_ptr<VectorKernelArgumentInfo>& outInfo);
    void parse(const std::shared_ptr<StructureKernelArgumentInfo>& outInfo);
    void parse(const std::shared_ptr<ImageKernelArgumentInfo>& outInfo);
    void parse(const std::shared_ptr<SamplerKernelArgumentInfo>& outInfo);
    void parse(const std::shared_ptr<EventKernelArgumentInfo>& outInfo);
    void parse(const std::shared_ptr<PointerKernelArgumentInfo>& outInfo);

    int argIndex;
    std::string argName;
    std::string argTypeName;
    std::string argType;
    bool unused = false;

    std::vector<std::string>::const_iterator restParametersBeginIter;
    std::vector<std::string>::const_iterator restParametersEndIter;

    std::string asmLine;
};

class KernelArgumentInfoParseError : public std::runtime_error {
   public:
    explicit KernelArgumentInfoParseError(const std::string& arg)
        : std::runtime_error(arg){};
    explicit KernelArgumentInfoParseError(const char* arg)
        : std::runtime_error(arg){};
};