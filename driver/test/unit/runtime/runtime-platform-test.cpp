#include <common/test/doctest.h>

#include <runtime/runtime-commons.h>
#include <vector>

#include "runtime/icd/icd.h"
#include "unit-test-common/test-commons.h"

TEST_SUITE("Platform API") {
    TEST_CASE("clGetPlatformIDs") {
        SUBCASE("should be able to return platform count") {
            cl_uint platformCount;
            const auto errorCode = clGetPlatformIDs(0, nullptr, &platformCount);

            CHECK(errorCode == CL_SUCCESS);
            CHECK(platformCount == 1);
        }

        SUBCASE("should return platform list") {
            auto platformList = std::vector<cl_platform_id>(1);
            const auto errorCode =
                clGetPlatformIDs(1, platformList.data(), nullptr);

            CHECK(errorCode == CL_SUCCESS);
            CHECK(platformList[0] != nullptr);
        }

        SUBCASE("platform should have correct vendor") {
            CHECK(test::getPlatform()->vendor == "sudo-team-company");
        }

        SUBCASE("platform should have correct name") {
            CHECK(test::getPlatform()->name ==
                  "AMD Accelerated Parallel Processing");
        }

        SUBCASE("platform should have cl_khr_icd extension") {
            CHECK(test::getPlatform()->extensions == "cl_khr_icd");
        }

        SUBCASE("should fail with incorrect parameters") {
            cl_uint errorCode = 0;

            errorCode = clGetPlatformIDs(1, nullptr, nullptr);
            CHECK(errorCode == CL_INVALID_VALUE);

            errorCode = clGetPlatformIDs(0, {}, nullptr);
            CHECK(errorCode == CL_INVALID_VALUE);
        }
    }

    TEST_CASE("clGetPlatformInfo") {
        SUBCASE("should correctly return platform param") {
            const std::string expected = "sudo-team-company";

            auto platform = test::getPlatform();

            char actual[128];
            const auto errorCode = clGetPlatformInfo(
                platform, CL_PLATFORM_VENDOR, 128, actual, nullptr);

            CHECK(errorCode == CL_SUCCESS);
            CHECK(std::string(actual) == expected);
        }

        SUBCASE("should fail if platform is null") {
            const auto errorCode =
                clGetPlatformInfo(nullptr, CL_PLATFORM_VENDOR, 0, {}, nullptr);

            CHECK(errorCode == CL_INVALID_PLATFORM);
        }
    }
}
