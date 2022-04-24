#include <cstring>
#include <functional>
#include <numeric>
#include <string>
#include <unordered_set>
#include <utility>

#include "CL/cl_ext2.h"
#include "DeviceConfigurationParser.h"
#include "common/utils/common.hpp"
#include "runtime/common/runtime-commons.h"
#include "runtime/icd/CLPlatformId.hpp"

template <typename T>
T parseNumber(const std::string& value);

cl_bool parseClBool(const std::string& value);
cl_bitfield parseBitfield(
    const std::string& value,
    std::function<cl_bitfield(const std::string& value)> parseFunction);

template <typename T>
std::vector<T> parseArray(const std::string& value,
                          T parseFunction(const std::string& value));

cl_device_local_mem_type parseDeviceLocalMemType(const std::string& value);
cl_device_exec_capabilities parseDeviceExecCapabilities(
    const std::string& value);
cl_command_queue_properties parseCommandQueueProperties(
    const std::string& value);
cl_device_type parseDeviceType(const std::string& value);
cl_device_mem_cache_type parseDeviceMemCacheType(const std::string& value);
cl_device_partition_property parseDevicePartitionProperty(
    const std::string& value);
cl_device_affinity_domain parseDeviceAffinityDomain(const std::string& value);
cl_device_fp_config parseDeviceFpConfig(const std::string& value);

void hardcodeParameter(
    std::unordered_map<cl_device_info, CLObjectInfoParameterValue>&
        outParameters,
    cl_device_info name,
    CLObjectInfoParameterValueType value,
    size_t size) {
    utils::insertOrUpdate<cl_device_info>(
        outParameters, name,
        CLObjectInfoParameterValue(std::move(value), size));
}

void DeviceConfigurationParser::load(const std::string& configurationFilePath) {
    std::ifstream configurationFile(configurationFilePath);

    if (!configurationFile.is_open()) {
        throw DeviceConfigurationParseError("Failed to open " +
                                            configurationFilePath);
    }

    std::unordered_map<cl_device_info, CLObjectInfoParameterValue> parameters;
    std::string line;

    while (getline(configurationFile, line)) {
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        const auto [parameterName, parameterValue] = utils::splitTwo(line, '=');
        const auto parsedParameter = parseParameter(
            utils::trim(parameterName), utils::trim(parameterValue));

        parameters.emplace(parsedParameter.name, parsedParameter.value);
    }

    hardcodeParameter(parameters, CL_DEVICE_PLATFORM, kPlatform,
                      sizeof(cl_platform_id));

    hardcodeParameter(parameters, CL_DEVICE_VENDOR_ID,
                      reinterpret_cast<void*>(0x1002), sizeof(cl_uint));

    hardcodeParameter(parameters, CL_DEVICE_PARENT_DEVICE, nullptr,
                      sizeof(cl_device_id));

    hardcodeParameter(parameters, CL_DEVICE_OPENCL_C_VERSION,
                      kPlatform->openClVersion,
                      kPlatform->openClVersion.size());

    hardcodeParameter(parameters, CL_DRIVER_VERSION, kPlatform->driverVersion,
                      kPlatform->driverVersion.size() + 1);

    const auto deviceVersion = std::string(kPlatform->openClVersion) +
                               " AMD-APP (" + kPlatform->driverVersion + ")";
    hardcodeParameter(parameters, CL_DEVICE_VERSION, deviceVersion,
                      deviceVersion.size() + 1);

    hardcodeParameter(parameters, CL_DEVICE_AVAILABLE, (void*) true,
                      sizeof(cl_bool));

    hardcodeParameter(parameters, CL_DEVICE_LINKER_AVAILABLE, (void*) false,
                      sizeof(cl_bool));

    hardcodeParameter(parameters, CL_DEVICE_COMPILER_AVAILABLE, (void*) false,
                      sizeof(cl_bool));

    hardcodeParameter(parameters, CL_DEVICE_PARTITION_MAX_SUB_DEVICES,
                      reinterpret_cast<void*>(0), sizeof(cl_uint));

    hardcodeParameter(parameters, CL_DEVICE_IMAGE_SUPPORT, (void*) false,
                      sizeof(cl_bool));

    hardcodeParameter(parameters, CL_DEVICE_HOST_UNIFIED_MEMORY, (void*) true,
                      sizeof(cl_bool));

    hardcodeParameter(parameters, CL_DEVICE_ERROR_CORRECTION_SUPPORT,
                      (void*) false, sizeof(cl_bool));

    std::string extensions = kPlatform->extensions;
    if (parameters.find(CL_DEVICE_EXTENSIONS) != parameters.end()) {
        extensions += " " + std::get<std::string>(
                                parameters.at(CL_DEVICE_EXTENSIONS).value);
    }
    hardcodeParameter(parameters, CL_DEVICE_EXTENSIONS, extensions,
                      extensions.size());

    mConfigurationPath = configurationFilePath;
    // TODO: memory leak of arrays
    mParameters.clear();
    mParameters = parameters;
}

std::optional<CLObjectInfoParameterValue>
DeviceConfigurationParser::getParameter(cl_device_info parameter) const {
    if (mParameters.find(parameter) != mParameters.end()) {
        return mParameters.at(parameter);
    }

    const auto isBasicOpenCLParameter =
        parameter >= CL_DEVICE_TYPE &&
        parameter <= CL_DEVICE_PRINTF_BUFFER_SIZE;

    const auto isAmdParameter =
        parameter >= CL_DEVICE_PROFILING_TIMER_OFFSET_AMD &&
        parameter <= CL_DEVICE_LOCAL_MEM_BANKS_AMD;

    const auto isValidOpenCLParameter =
        isBasicOpenCLParameter || isAmdParameter;

    if (isValidOpenCLParameter) {
        kLogger.warn("Parameter " + std::to_string(parameter) +
                     " was not found in config");
        return CLObjectInfoParameterValue(nullptr, 0);
    }

    return std::nullopt;
}

#define PARSE_PARAMETER_WITH_BODY(param, fn) \
    if (parameterName == #param) {           \
        clParameter = param;                 \
        fn();                                \
    }

#define PARSE_PARAMETER(param, type, parseFn)                      \
    if (parameterName == #param) {                                 \
        clParameter = param;                                       \
        resultSize = sizeof(type);                                 \
        result = reinterpret_cast<void*>(parseFn(parameterValue)); \
    }

#define PARSE_BITFIELD_PARAMETER(param, type, parseFn)                       \
    if (parameterName == #param) {                                           \
        clParameter = param;                                                 \
        resultSize = sizeof(type);                                           \
        result =                                                             \
            reinterpret_cast<void*>(parseBitfield(parameterValue, parseFn)); \
    }

#define PARSE_NUMBER_PARAMETER(param, type)                                  \
    if (parameterName == #param) {                                           \
        clParameter = param;                                                 \
        resultSize = sizeof(type);                                           \
        result = reinterpret_cast<void*>(parseNumber<type>(parameterValue)); \
    }

#define PARSE_BOOL_PARAMETER(param)                                    \
    if (parameterName == #param) {                                     \
        clParameter = param;                                           \
        resultSize = sizeof(cl_bool);                                  \
        result = reinterpret_cast<void*>(parseClBool(parameterValue)); \
    }

#define PARSE_STRING_PARAMETER(param)                            \
    if (parameterName == #param) {                               \
        clParameter = param;                                     \
        result = parameterValue == "\"\"" ? "" : parameterValue; \
    }

#define IGNORE_PARAMETER(param)                                           \
    if (parameterName == #param) {                                        \
        kLogger.warn(std::string("Ignoring config parameter ") + #param); \
        clParameter = param;                                              \
        result = nullptr;                                                 \
    }

DeviceConfigurationParser::ParsedParameter
DeviceConfigurationParser::parseParameter(const std::string& parameterName,
                                          const std::string& parameterValue) {
    size_t resultSize = 0;
    CLObjectInfoParameterValueType result;
    bool isPointer = false;
    cl_device_info clParameter = 0;

    PARSE_PARAMETER(CL_DEVICE_TYPE, cl_device_type, parseDeviceType)
    PARSE_STRING_PARAMETER(CL_DEVICE_NAME)
    //    PARSE_NUMBER_PARAMETER(
    //        CL_DEVICE_VENDOR_ID,
    //        cl_uint)  // TODO(parseParameter): parse hex correctly
    PARSE_STRING_PARAMETER(CL_DEVICE_VENDOR)
    PARSE_STRING_PARAMETER(CL_DEVICE_PROFILE)
    PARSE_STRING_PARAMETER(CL_DEVICE_VERSION)
    PARSE_STRING_PARAMETER(CL_DEVICE_EXTENSIONS)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_COMPUTE_UNITS, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, cl_uint)
    PARSE_PARAMETER_WITH_BODY(CL_DEVICE_MAX_WORK_ITEM_SIZES, [&]() {
        // TODO: get actual CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS value
        const std::vector<size_t> values =
            parseArray(parameterValue, parseNumber<size_t>);
        resultSize = values.size() * sizeof(size_t);
        auto* valuesArray = new size_t[values.size()];
        memcpy(valuesArray, values.data(), resultSize);
        result = valuesArray;
        isPointer = true;
    })
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_WORK_GROUP_SIZE, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_CLOCK_FREQUENCY, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_ADDRESS_BITS, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_MEM_ALLOC_SIZE, cl_ulong)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_READ_IMAGE_ARGS, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_WRITE_IMAGE_ARGS, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE2D_MAX_WIDTH, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE2D_MAX_HEIGHT, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE3D_MAX_WIDTH, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE3D_MAX_HEIGHT, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE3D_MAX_DEPTH, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_SAMPLERS, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_PARAMETER_SIZE, size_t)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MEM_BASE_ADDR_ALIGN, cl_uint)
    PARSE_BITFIELD_PARAMETER(CL_DEVICE_SINGLE_FP_CONFIG, cl_device_fp_config,
                             parseDeviceFpConfig)
    PARSE_BITFIELD_PARAMETER(CL_DEVICE_DOUBLE_FP_CONFIG, cl_device_fp_config,
                             parseDeviceFpConfig)
    PARSE_PARAMETER(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, cl_device_mem_cache_type,
                    parseDeviceMemCacheType)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, cl_ulong)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_GLOBAL_MEM_SIZE, cl_ulong)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, cl_ulong)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_CONSTANT_ARGS, cl_uint)
    PARSE_PARAMETER(CL_DEVICE_LOCAL_MEM_TYPE, cl_device_local_mem_type,
                    parseDeviceLocalMemType)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_LOCAL_MEM_SIZE, cl_ulong)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PROFILING_TIMER_RESOLUTION, size_t)
    PARSE_PARAMETER(CL_DEVICE_ENDIAN_LITTLE, cl_bool, parseClBool)
    PARSE_BITFIELD_PARAMETER(CL_DEVICE_EXECUTION_CAPABILITIES,
                             cl_device_exec_capabilities,
                             parseDeviceExecCapabilities)
    PARSE_PARAMETER_WITH_BODY(CL_DEVICE_BUILT_IN_KERNELS, [&]() {
        result = parameterValue == "0" ? "" : parameterValue.c_str();
    })
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PRINTF_BUFFER_SIZE, size_t)
    PARSE_BOOL_PARAMETER(CL_DEVICE_PREFERRED_INTEROP_USER_SYNC)
    PARSE_PARAMETER(CL_DEVICE_PARTITION_PROPERTIES,
                    cl_device_partition_property, parseDevicePartitionProperty)
    PARSE_BITFIELD_PARAMETER(CL_DEVICE_PARTITION_AFFINITY_DOMAIN,
                             cl_device_affinity_domain,
                             parseDeviceAffinityDomain)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PARTITION_TYPE, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_REFERENCE_COUNT, cl_uint)
    PARSE_PARAMETER(CL_DEVICE_QUEUE_PROPERTIES, cl_command_queue_properties,
                    parseCommandQueueProperties)

    /* * * * *
     * AMD extensions
     * https://www.khronos.org/registry/OpenCL/extensions/amd/cl_amd_device_attribute_query.txt
     * * * * */
    PARSE_NUMBER_PARAMETER(CL_DEVICE_PROFILING_TIMER_OFFSET_AMD, cl_uint)
    PARSE_PARAMETER_WITH_BODY(CL_DEVICE_TOPOLOGY_AMD, [&]() {
        resultSize = sizeof(cl_device_topology_amd);

        // PCI-E, 0000:00:04.0
        const auto info = utils::split(parameterValue, ':');

        const auto errorString =
            "Invalid topology! Should follow format \"PCI-E, "
            "xxxx:bb:dd.f\" where x -- domain, b -- bus, d -- device, f -- "
            "function";
        if (info.size() != 3) {
            throw DeviceConfigurationParseError(errorString);
        }

        const auto suffix = utils::split(info[2], '.');

        if (suffix.size() != 2) {
            throw DeviceConfigurationParseError(errorString);
        }

        const auto bus = info[1];
        const auto device = suffix[0];
        const auto function = suffix[1];

        auto topology = new cl_device_topology_amd();
        topology->pcie.type = CL_DEVICE_TOPOLOGY_TYPE_PCIE_AMD;
        topology->pcie.bus = parseNumber<cl_char>(bus);
        topology->pcie.device = parseNumber<cl_char>(device);
        topology->pcie.function = parseNumber<cl_char>(function);

        result = topology;
        isPointer = true;
    })
    PARSE_STRING_PARAMETER(CL_DEVICE_BOARD_NAME_AMD)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_SIMD_PER_COMPUTE_UNIT_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_SIMD_WIDTH_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_SIMD_INSTRUCTION_WIDTH_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_WAVEFRONT_WIDTH_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_GLOBAL_MEM_CHANNELS_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_GLOBAL_MEM_CHANNEL_BANKS_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_GLOBAL_MEM_CHANNEL_BANK_WIDTH_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_LOCAL_MEM_SIZE_PER_COMPUTE_UNIT_AMD,
                           cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_LOCAL_MEM_BANKS_AMD, cl_uint)
    PARSE_NUMBER_PARAMETER(CL_DEVICE_MAX_GLOBAL_VARIABLE_SIZE, cl_uint)

    IGNORE_PARAMETER(CL_DEVICE_PLATFORM)
    IGNORE_PARAMETER(CL_DEVICE_VENDOR_ID)
    IGNORE_PARAMETER(CL_DEVICE_PARENT_DEVICE)
    IGNORE_PARAMETER(CL_DEVICE_OPENCL_C_VERSION)
    IGNORE_PARAMETER(CL_DRIVER_VERSION)
    IGNORE_PARAMETER(CL_DEVICE_PARTITION_MAX_SUB_DEVICES)
    IGNORE_PARAMETER(CL_DEVICE_AVAILABLE)
    IGNORE_PARAMETER(CL_DEVICE_LINKER_AVAILABLE)
    IGNORE_PARAMETER(CL_DEVICE_COMPILER_AVAILABLE)
    IGNORE_PARAMETER(CL_DEVICE_IMAGE_SUPPORT)
    IGNORE_PARAMETER(CL_DEVICE_HOST_UNIFIED_MEMORY)
    IGNORE_PARAMETER(CL_DEVICE_ERROR_CORRECTION_SUPPORT)
    IGNORE_PARAMETER(CL_DEVICE_GLOBAL_FREE_MEMORY_AMD)

    if (!clParameter) {
        kLogger.warn("Unknown config parameter: " + parameterName);
    }

    if (std::holds_alternative<std::string>(result)) {
        resultSize = strlen(std::get<std::string>(result).c_str()) + 1;
    }

    return ParsedParameter(
        clParameter, CLObjectInfoParameterValue(result, resultSize, isPointer));
}

template <typename T>
T parseNumber(const std::string& value) {
    try {
        return static_cast<T>(std::stoul(value));
    } catch (std::invalid_argument& e) {
        throw DeviceConfigurationParseError("Failed to parse number: " + value);
    }
}

cl_bitfield parseBitfield(
    const std::string& value,
    std::function<cl_bitfield(const std::string& value)> parseFunction) {
    // TODO(parseBitfield): parse 0xNN?
    const auto splitBitfield = utils::split(value, '|');

    const auto accumulator = [&](cl_bitfield acc, const auto& value) {
        return acc | parseFunction(utils::trim(value));
    };

    return std::accumulate(splitBitfield.begin(), splitBitfield.end(), 0,
                           accumulator);
}

template <typename T>
std::vector<T> parseArray(const std::string& value,
                          T parseFunction(const std::string&)) {
    const auto splitValue = utils::split(value, ' ');

    std::vector<T> result;
    std::transform(splitValue.begin(), splitValue.end(),
                   std::back_inserter(result), [&](auto& value) {
                       return parseFunction(utils::trim(value));
                   });

    return result;
}

cl_bool parseClBool(const std::string& value) {
    if (value == "CL_TRUE") {
        return CL_TRUE;
    } else if (value == "CL_FALSE") {
        return CL_FALSE;
    }

    throw DeviceConfigurationParseError("Failed to parse cl_bool: " + value);
}

cl_device_type parseDeviceType(const std::string& value) {
    if (value == "CL_DEVICE_TYPE_CPU") {
        return CL_DEVICE_TYPE_CPU;

    } else if (value == "CL_DEVICE_TYPE_GPU") {
        return CL_DEVICE_TYPE_GPU;

    } else if (value == "CL_DEVICE_TYPE_ACCELERATOR") {
        return CL_DEVICE_TYPE_ACCELERATOR;

    } else if (value == "CL_DEVICE_TYPE_DEFAULT") {
        return CL_DEVICE_TYPE_DEFAULT;

    } else if (value == "CL_DEVICE_TYPE_CUSTOM") {
        return CL_DEVICE_TYPE_CUSTOM;
    }

    throw DeviceConfigurationParseError("Unknown cl_device_type value: " +
                                        value);
}

cl_device_local_mem_type parseDeviceLocalMemType(const std::string& value) {
    if (value == "CL_LOCAL") {
        return CL_LOCAL;

    } else if (value == "CL_GLOBAL") {
        return CL_GLOBAL;

    } else if (value == "CL_NONE") {
        return CL_NONE;
    }

    throw DeviceConfigurationParseError(
        "Unknown cl_device_local_mem_type value: " + value);
}

cl_device_exec_capabilities parseDeviceExecCapabilities(
    const std::string& value) {
    if (value == "CL_EXEC_KERNEL") {
        return CL_EXEC_KERNEL;

    } else if (value == "CL_EXEC_NATIVE_KERNEL") {
        return CL_EXEC_NATIVE_KERNEL;
    }

    throw DeviceConfigurationParseError(
        "Unknown cl_device_exec_capabilities value: " + value);
}

cl_command_queue_properties parseCommandQueueProperties(
    const std::string& value) {
    if (value == "CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE") {
        return CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;

    } else if (value == "CL_QUEUE_PROFILING_ENABLE") {
        return CL_QUEUE_PROFILING_ENABLE;
    }

    throw DeviceConfigurationParseError(
        "Unknown cl_command_queue_properties value: " + value);
}

cl_device_mem_cache_type parseDeviceMemCacheType(const std::string& value) {
    if (value == "CL_READ_ONLY_CACHE") {
        return CL_READ_ONLY_CACHE;

    } else if (value == "CL_READ_WRITE_CACHE") {
        return CL_READ_WRITE_CACHE;

    } else if (value == "CL_NONE") {
        return CL_NONE;
    }

    throw DeviceConfigurationParseError(
        "Unknown cl_device_mem_cache_type value: " + value);
}

cl_device_partition_property parseDevicePartitionProperty(
    const std::string& value) {
    // TODO(parseDevicePartitionProperty): parse array

    if (value == "CL_DEVICE_PARTITION_EQUALLY") {
        return CL_DEVICE_PARTITION_EQUALLY;

    } else if (value == "CL_DEVICE_PARTITION_BY_COUNTS") {
        return CL_DEVICE_PARTITION_BY_COUNTS;

    } else if (value == "CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN") {
        return CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN;

    } else if (value == "CL_NONE" || value == "0") {
        return CL_NONE;
    }

    throw DeviceConfigurationParseError(
        "Unknown cl_device_partition_property value: " + value);
}

cl_device_affinity_domain parseDeviceAffinityDomain(const std::string& value) {
    if (value == "CL_DEVICE_AFFINITY_DOMAIN_NUMA") {
        return CL_DEVICE_AFFINITY_DOMAIN_NUMA;

    } else if (value == "CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE") {
        return CL_DEVICE_AFFINITY_DOMAIN_L4_CACHE;

    } else if (value == "CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE") {
        return CL_DEVICE_AFFINITY_DOMAIN_L3_CACHE;

    } else if (value == "CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE") {
        return CL_DEVICE_AFFINITY_DOMAIN_L2_CACHE;

    } else if (value == "CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE") {
        return CL_DEVICE_AFFINITY_DOMAIN_L1_CACHE;

    } else if (value == "CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE") {
        return CL_DEVICE_AFFINITY_DOMAIN_NEXT_PARTITIONABLE;

    } else if (value == "CL_NONE" || value == "0") {
        return CL_NONE;
    }

    throw DeviceConfigurationParseError(
        "Unknown cl_device_affinity_domain value: " + value);
}

cl_device_fp_config parseDeviceFpConfig(const std::string& value) {
    if (value == "CL_FP_DENORM") {
        return CL_FP_DENORM;

    } else if (value == "CL_FP_INF_NAN") {
        return CL_FP_INF_NAN;

    } else if (value == "CL_FP_ROUND_TO_NEAREST") {
        return CL_FP_ROUND_TO_NEAREST;

    } else if (value == "CL_FP_ROUND_TO_ZERO") {
        return CL_FP_ROUND_TO_ZERO;

    } else if (value == "CL_FP_ROUND_TO_INF") {
        return CL_FP_ROUND_TO_INF;

    } else if (value == "CL_FP_FMA") {
        return CL_FP_FMA;

    } else if (value == "CL_FP_SOFT_FLOAT") {
        return CL_FP_SOFT_FLOAT;

    } else if (value == "CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT") {
        return CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT;

    } else if (value == "CL_NONE" || value == "0") {
        return CL_NONE;
    }

    throw DeviceConfigurationParseError("Unknown cl_device_fp_config value: " +
                                        value);
}
