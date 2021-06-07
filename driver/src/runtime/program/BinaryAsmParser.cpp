#include <runtime/common/runtime-commons.h>
#include <cassert>
#include <common/utils/common.hpp>
#include <string>

#include "BinaryAsmParser.h"
#include "KernelArgumentInfoParser.h"

std::unique_ptr<BinaryDisassemblingResult> BinaryAsmParser::parseAsm() {
    if (alreadyParsed) {
        throw std::runtime_error(
            "Binary was parsed before so parser is invalidated.");
    }

    parsingResult = std::make_unique<BinaryDisassemblingResult>();

    auto lines = utils::splitMap<std::string>(*input, '\n', [](auto line) {
        utils::trimInplace(line);
        return line;
    });

    std::string parameters;

    for (const auto& line : lines) {
        if (line.empty()) {
            continue;
        }

        std::vector<std::string> splitLine;
        if (parsingState == KernelInstructions &&
            utils::startsWith(line, "/*")) {
            splitLine = utils::split(line, ' ', 2);

        } else {
            splitLine = utils::split(line, ' ', 1);
        }

        if (splitLine.size() == 1) {
            parseSingleInstruction(splitLine[0]);

        } else if (splitLine.size() == 2) {
            parseParameter(line, splitLine[0], splitLine[1]);

        } else {
            parseKernelInstruction(line, splitLine[0], splitLine[1],
                                   splitLine[2]);
        }
    }

    if (currentKernelBuilder) {
        parsingResult->kernelBuilders.push_back(std::move(currentKernelBuilder));
    }

    alreadyParsed = true;

    return std::move(parsingResult);
}

void BinaryAsmParser::parseSingleInstruction(const std::string& instruction) {
    if (instruction == ".config") {
        parsingState = KernelConfig;
        return;
    }

    if (instruction == ".text") {
        parsingState = KernelInstructions;
        return;
    }

    switch (parsingState) {
        case BinaryParameters: {
            assert(currentKernelBuilder == nullptr);
            parseBinaryParameter(instruction, instruction, "");
            break;
        }

        case KernelConfig: {
            assert(currentKernelBuilder != nullptr);
            parseKernelConfigParameter(instruction, instruction, "");
            break;
        }

        case KernelInstructions: {
            assert(currentKernelBuilder != nullptr);
            parseKernelInstruction(instruction, "", instruction, "");
            break;
        }
    }
}

void BinaryAsmParser::parseParameter(const std::string& line,
                                     const std::string& parameterName,
                                     const std::string& parameterValue) {
    if (utils::startsWith(parameterName, ".kernel")) {
        if (currentKernelBuilder) {
            parsingResult->kernelBuilders.push_back(
                std::move(currentKernelBuilder));
        }

        currentKernelBuilder = std::make_unique<CLKernelBuilder>();
        currentKernelBuilder->name = parameterValue;
        return;
    }

    switch (parsingState) {
        case BinaryParameters: {
            assert(currentKernelBuilder == nullptr);
            parseBinaryParameter(line, parameterName, parameterValue);
            break;
        }

        case KernelConfig: {
            assert(currentKernelBuilder != nullptr);
            parseKernelConfigParameter(line, parameterName, parameterValue);
            break;
        }

        case KernelInstructions: {
            assert(currentKernelBuilder != nullptr);
            if (utils::startsWith(line, "/*")) {
                parseKernelInstruction(line, parameterName, parameterValue, "");
            } else {
                kLogger.warn(
                    "Got instruction without address while parsing kernel "
                    "instructions: " +
                    line);
                parseKernelInstruction(line, "/*unknown*/", parameterName,
                                       parameterValue);
            }
        }
    }
}

void BinaryAsmParser::parseKernelInstruction(const std::string& line,
                                             const std::string& address,
                                             const std::string& instruction,
                                             const std::string& values) {
    if (instruction[0] == '.') {
        // TODO: this is label
        currentKernelBuilder->instructions.push_back(line);
        return;
    }

    assert(utils::startsWith(address, "/*"));
    assert(utils::endsWith(address, "*/"));
    const auto addressValue = address.substr(2, address.size() - 4);
    currentKernelBuilder->instructions.push_back(line);
}

void BinaryAsmParser::parseBinaryParameter(const std::string& line,
                                           const std::string& parameterName,
                                           const std::string& parameterValue) {
    if (parameterName == ".gpu") {
        parsingResult->gpuName = parameterValue;

    } else if (parameterName == ".compile_options") {
        parsingResult->compileOptions = parameterValue;

    } else {
        parsingResult->parameters.push_back(line);
    }
}

void BinaryAsmParser::parseKernelConfigParameter(
    const std::string& line,
    const std::string& parameterName,
    const std::string& parameterValue) {
    // clang formatting is meh, so here is a comment separating args from body

    if (parameterName == ".cws" || parameterName == ".reqd_work_group_size") {
        const auto values =
            utils::splitMap<size_t>(parameterValue, ',', [](const auto value) {
                return std::stoul(utils::trim(value));
            });

        currentKernelBuilder->requiredWorkGroupSize.x = values[0];

        if (values.size() > 1) {
            currentKernelBuilder->requiredWorkGroupSize.y = values[1];
        }

        if (values.size() > 2) {
            currentKernelBuilder->requiredWorkGroupSize.z = values[2];
        }
    }

    if (parameterName == ".arg" && !utils::startsWith(parameterValue, "_")) {
        parseKernelArgument(parameterValue);
    }

    currentKernelBuilder->config.push_back(line);
}

void BinaryAsmParser::parseKernelArgument(
    const std::string& argumentConfigLine) {
    auto parser = KernelArgumentInfoParser(
        currentKernelBuilder->argumentInfo.size(), argumentConfigLine);
    currentKernelBuilder->argumentInfo.push_back(parser.parse());
}
