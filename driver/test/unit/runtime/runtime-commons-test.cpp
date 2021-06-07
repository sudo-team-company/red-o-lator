#include <common/test/doctest.h>
#include <common/utils/common.hpp>
#include <string>

#include "runtime/common/runtime-commons.h"

TEST_SUITE("Runtime commons test") {
    TEST_CASE("getParamInfo") {
        SUBCASE(
            "should do nothing if param_value and param_value_size_ret are not "
            "set") {
            auto errorCode =
                getParamInfo(CL_PLATFORM_NAME, 0, nullptr, nullptr, []() {
                    return std::nullopt;
                });

            REQUIRE(errorCode == CL_SUCCESS);
        }

        SUBCASE("should be able to get param value size") {
            size_t paramValueSize = 0;
            std::string paramValue = "Test value";
            size_t resultSize = strlen(paramValue.c_str()) + 1;

            auto errorCode = getParamInfo(
                CL_PLATFORM_NAME, 0, nullptr, &paramValueSize, [&]() {
                    return utils::optionalOf(
                        CLObjectInfoParameterValue(nullptr, resultSize));
                });

            REQUIRE(errorCode == CL_SUCCESS);
            REQUIRE(paramValueSize == resultSize);
        }

        SUBCASE("should be able to get param value") {
            std::string resultParam = "red-o-lator";
            auto resultParamSize = strlen(resultParam.c_str()) + 1;

            char paramValue[20];
            auto errorCode =
                getParamInfo(CL_PLATFORM_NAME, 20, paramValue, nullptr, [&]() {
                    return utils::optionalOf(CLObjectInfoParameterValue(
                        resultParam, resultParamSize));
                });

            REQUIRE(errorCode == CL_SUCCESS);
            REQUIRE(std::string(paramValue) == resultParam);
        }

        SUBCASE("should fail if parameter is not found") {
            char paramValue[20];
            auto errorCode =
                getParamInfo(CL_PLATFORM_NAME, 20, paramValue, nullptr, [&]() {
                    return std::nullopt;
                });

            REQUIRE(errorCode == CL_INVALID_VALUE);
        }

        SUBCASE("should fail if parameter size was less than returned one") {
            std::string resultParam = "red-o-lator";
            auto resultParamSize = strlen(resultParam.c_str()) + 1;

            auto paramSize = resultParamSize - 1;
            auto paramValue = new char[paramSize];
            auto errorCode = getParamInfo(
                CL_PLATFORM_NAME, paramSize, paramValue, nullptr, [&]() {
                    return utils::optionalOf(CLObjectInfoParameterValue(
                        resultParam, resultParamSize));
                });

            REQUIRE(errorCode == CL_INVALID_VALUE);
        }
    }

    TEST_CASE("hasMutuallyExclusiveFlags") {
        SUBCASE("should return false if no mutually exclusive flags found") {
            auto flags =
                CL_MEM_USE_HOST_PTR | CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS;

            auto actual = utils::hasMutuallyExclusiveFlags(
                flags, {CL_MEM_USE_HOST_PTR, CL_MEM_COPY_HOST_PTR});

            REQUIRE_FALSE(actual);
        }

        SUBCASE("should return true if found mutually exclusive flags") {
            auto flags =
                CL_MEM_USE_HOST_PTR | CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_NO_ACCESS;

            auto actual = utils::hasMutuallyExclusiveFlags(
                flags, {CL_MEM_USE_HOST_PTR, CL_MEM_COPY_HOST_PTR});

            REQUIRE(actual);
        }
    }
}