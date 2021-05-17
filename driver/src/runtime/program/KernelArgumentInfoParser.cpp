#include <common/common.hpp>

#include "KernelArgumentInfoParser.h"

#define PARSE_ARGUMENT(type)                       \
    do {                                           \
        const auto out = std::make_shared<type>(); \
        parse(out);                                \
        result = out;                              \
    } while (0);

std::shared_ptr<KernelArgumentInfo> KernelArgumentInfoParser::parse() {
    auto splitLine =
        utils::splitMap<std::string>(asmLine, ',', -1, [](auto arg) {
            return utils::trim(arg);
        });

    const auto size = splitLine.size();
    auto shift = 0;

    argName = splitLine[0];

    if (splitLine[1][0] == '"') {
        argTypeName = splitLine[1].substr(1, splitLine[1].size() - 2);
    } else {
        shift++;
    }

    if (splitLine[size - 1] == "unused") {
        unused = true;
    }

    argType = splitLine[2 - shift];

    restParametersBeginIter = splitLine.begin() + 2 - shift + 1;
    restParametersEndIter = unused ? splitLine.end() - 1 : splitLine.end();

    std::shared_ptr<KernelArgumentInfo> result;
    if (utils::endsWith(argType, "*")) {
        PARSE_ARGUMENT(PointerKernelArgumentInfo)

    } else if (std::any_of(ScalarKernelArgumentInfo::typeNameVariants.begin(),
                           ScalarKernelArgumentInfo::typeNameVariants.end(),
                           [&](auto value) {
                               return utils::startsWith(argType, value);
                           })) {
        PARSE_ARGUMENT(ScalarKernelArgumentInfo)

    } else if (utils::contains(StructureKernelArgumentInfo::typeNameVariants,
                               argType)) {
        PARSE_ARGUMENT(StructureKernelArgumentInfo)

    } else if (utils::contains(ImageKernelArgumentInfo::typeNameVariants,
                               argType)) {
        PARSE_ARGUMENT(ImageKernelArgumentInfo)

    } else if (utils::contains(SamplerKernelArgumentInfo::typeNameVariants,
                               argType)) {
        PARSE_ARGUMENT(SamplerKernelArgumentInfo)
    }

    result->index = argIndex;
    result->argumentName = argName;
    result->type = argType;
    result->typeName = argTypeName;

    return result;
}

void throwParseError(const std::string& argumentType,
                     const std::string& parameter) {
    const auto message = "Unknown parameter value for " + argumentType +
                         " kernel argument: " + parameter;
    throw KernelArgumentInfoParseError(message);
}

void KernelArgumentInfoParser::parse(
    const std::shared_ptr<ScalarKernelArgumentInfo>& outInfo) {
    for (auto it = restParametersBeginIter; it < restParametersEndIter; ++it) {
        const auto param = *it;

        if (param.empty()) {
            continue;
        }

        throwParseError("scalar", param);
    }
}

void KernelArgumentInfoParser::parse(
    const std::shared_ptr<StructureKernelArgumentInfo>& outInfo) {
    for (auto it = restParametersBeginIter; it < restParametersEndIter; ++it) {
        const auto param = *it;

        if (param.empty()) {
            continue;
        }

        if (std::isdigit(param[0])) {
            outInfo->structSize = std::stoi(param);
        } else {
            throwParseError("structure", param);
        }
    }
}

void KernelArgumentInfoParser::parse(
    const std::shared_ptr<ImageKernelArgumentInfo>& outInfo) {
    for (auto it = restParametersBeginIter; it < restParametersEndIter; ++it) {
        const auto param = *it;

        if (param.empty()) {
            continue;
        }

        if (param == "read_only" || param == "rdonly") {
            outInfo->accessQualifier = CL_KERNEL_ARG_ACCESS_READ_ONLY;

        } else if (param == "write_only" || param == "wronly") {
            outInfo->accessQualifier = CL_KERNEL_ARG_ACCESS_WRITE_ONLY;

        } else if (param == "read_write" || param == "rdwr") {
            outInfo->accessQualifier = CL_KERNEL_ARG_ACCESS_READ_WRITE;

        } else if (std::isdigit(param[0])) {
            outInfo->resId = std::stoi(param);

        } else {
            throwParseError("image", param);
        }
    }
}

void KernelArgumentInfoParser::parse(
    const std::shared_ptr<SamplerKernelArgumentInfo>& outInfo) {
    for (auto it = restParametersBeginIter; it < restParametersEndIter; ++it) {
        const auto param = *it;

        if (param.empty()) {
            continue;
        }

        if (std::isdigit(param[0])) {
            outInfo->resId = std::stoi(param);
        } else {
            throwParseError("sampler", param);
        }
    }
}

bool parsePointerAccessParam(
    const std::string& param,
    const std::shared_ptr<PointerKernelArgumentInfo>& outInfo) {
    if (param.empty()) {
        return false;
    }

    if (param == "const") {
        outInfo->argTypeQualifier = CL_KERNEL_ARG_TYPE_CONST;
        return true;

    } else if (param == "restrict") {
        outInfo->argTypeQualifier = CL_KERNEL_ARG_TYPE_RESTRICT;
        return true;

    } else if (param == "volatile") {
        outInfo->argTypeQualifier = CL_KERNEL_ARG_TYPE_VOLATILE;
        return true;
    }

    return false;
}

void KernelArgumentInfoParser::parse(
    const std::shared_ptr<PointerKernelArgumentInfo>& outInfo) {
    for (auto it = restParametersBeginIter; it < restParametersEndIter; ++it) {
        const auto param = *it;

        if (param.empty()) {
            continue;
        }

        const auto maybeSplit = utils::split(param, ' ');
        if (maybeSplit.size() > 1) {
            for (auto& accessParam : maybeSplit) {
                parsePointerAccessParam(accessParam, outInfo);
            }

            return;
        }

        if (param == "local") {
            outInfo->addressQualifier = CL_KERNEL_ARG_ADDRESS_LOCAL;

        } else if (param == "constant") {
            outInfo->addressQualifier = CL_KERNEL_ARG_ADDRESS_CONSTANT;

        } else if (param == "global") {
            outInfo->addressQualifier = CL_KERNEL_ARG_ADDRESS_GLOBAL;

        } else if (param == "read_only" || param == "rdonly") {
            outInfo->accessQualifier = CL_KERNEL_ARG_ACCESS_READ_ONLY;

        } else if (param == "write_only" || param == "wronly") {
            outInfo->accessQualifier = CL_KERNEL_ARG_ACCESS_WRITE_ONLY;

        } else if (param == "read_write" || param == "rdwr") {
            outInfo->accessQualifier = CL_KERNEL_ARG_ACCESS_READ_WRITE;

        } else if (std::isdigit(param[0]) &&
                   outInfo->accessQualifier == CL_KERNEL_ARG_ADDRESS_CONSTANT) {
            outInfo->constSize = std::stoi(param);

        } else if (std::isdigit(param[0])) {
            outInfo->structSize = std::stoi(param);

        } else if (!parsePointerAccessParam(param, outInfo)) {
            throwParseError("pointer", param);
        }
    }
}
