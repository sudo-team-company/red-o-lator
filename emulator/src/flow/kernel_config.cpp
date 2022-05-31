#include "kernel_config.h"


void validateSizes(size_t dims, const std::array<size_t, 3>& sizes, const char *name);

KernelConfig::KernelConfig(uint32_t dims,
                           const size_t *globalWorkOffset,
                           const size_t *globalWorkSize,
                           const size_t *localWorkSize,
                           const std::vector<std::string> &configParams)
    : dims(dims) {
    if (globalWorkOffset) {
        std::copy_n(globalWorkOffset, dims, this->globalWorkOffset.begin());
    }
    if (!globalWorkSize) {
        throw std::runtime_error("Global work size parameter can not be empty!");
    }
    validateSizes(dims, this->globalWorkSize, "Global work size");
    std::copy_n(globalWorkSize, dims, this->globalWorkSize.begin());
    if (localWorkSize) {
        std::copy_n(localWorkSize, dims, this->localWorkSize.begin());
        validateSizes(dims, this->localWorkSize, "Local work size");
    }
    for (auto &param: configParams) {
        auto dividedParam = utils::split(param.substr(1), ' ', 1);
        const auto &paramName = dividedParam[0];
        if ("sgprsnum" == paramName) {
            sgprsnum = std::stoi(dividedParam[1]);
        } else if ("vgprsnum" == paramName) {
            vgprsnum = std::stoi(dividedParam[1]);
        } else if ("priority" == paramName) {
            auto priorityValue = std::stoi(dividedParam[1]);
            assert(priorityValue >= 0 && priorityValue <= 3);
            priority = (uint8_t) priorityValue;
        } else if ("cws" == paramName) {
            auto workGroupSizes = utils::split(dividedParam[1], ',');
            this->localWorkSize = {std::stoul(workGroupSizes[0]),
                                   workGroupSizes.size() > 1 ? std::stoul(workGroupSizes[1]) : 1,
                                   workGroupSizes.size() > 2 ? std::stoul(workGroupSizes[2]) : 1};
        } else if ("pgmrsrc2" == paramName) {
            pgmrsrc2 = std::stoi(dividedParam[1], nullptr, 16);
        } else if ("dx10clamp" == paramName) {
            dx10clamp = true;
        } else if ("ieeemode" == paramName) {
            ieeemode = true;
        } else if ("useargs" == paramName) {
            useargs = true;
        } else if ("usegeneric" == paramName) {
            usegeneric = true;
        } else if ("usesetup" == paramName) {
            usesetup = true;
        } else {
            logger.warn(std::string("Unsupported configuration parameter: " + param));
        }
    }
    if (sgprsnum == 0 || vgprsnum == 0) {
        throw std::runtime_error("SGPRs and VGPRs amount should be specified");
    }
}

void validateSizes(size_t dims, const std::array<size_t, 3>& sizes, const char *name) {
    const char * dimsLetters = "XYZ";
    for (size_t i = 0; i < dims; ++i) {
        if (sizes[i] == 0) {
            throw std::runtime_error(std::string("Invalid ") + name + " for " + dimsLetters[i]);
        }
    }
}