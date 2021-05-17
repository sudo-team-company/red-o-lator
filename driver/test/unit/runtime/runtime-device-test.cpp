#include <common/test/doctest.h>
#include <runtime/runtime-commons.h>
#include <unit-test-common/test-commons.h>
#include <vector>

#include "runtime/icd/icd.h"

#include <CL/cl.hpp>

#define CHECK_DEVICE_PARAMETER(deviceParameter, expected) \
    CHECK(cl::Device(test::getDevice()).getInfo<deviceParameter>() == expected)

#define CHECK_DEVICE_STRING_PARAMETER(deviceParameter, expected)       \
    do {                                                               \
        char result[128];                                              \
        const auto errorCode = clGetDeviceInfo(                        \
            test::getDevice(), deviceParameter, 128, result, nullptr); \
        CHECK(errorCode == CL_SUCCESS);                                \
        CHECK(std::string(result) == expected);                        \
    } while (0)

TEST_SUITE("Device API") {
    TEST_CASE("clGetDeviceIDs") {
        SUBCASE("should be able to return device count") {
            cl_uint devicesCount;
            const auto errorCode =
                clGetDeviceIDs(test::getPlatform(), CL_DEVICE_TYPE_ALL, 0,
                               nullptr, &devicesCount);

            REQUIRE(errorCode == CL_SUCCESS);
            REQUIRE(devicesCount == 1);
        }

        SUBCASE("should return device list") {
            cl_platform_id platform = test::getPlatform();
            auto deviceList = std::vector<cl_device_id>(1);
            const auto errorCode = clGetDeviceIDs(
                platform, CL_DEVICE_TYPE_ALL, 1, deviceList.data(), nullptr);

            REQUIRE(errorCode == CL_SUCCESS);
            REQUIRE(deviceList[0] == kDevice);
        }

        SUBCASE("should fail if platfrom is null") {
            const auto errorCode = clGetDeviceIDs(nullptr, CL_DEVICE_TYPE_ALL,
                                                  1, nullptr, nullptr);

            REQUIRE(errorCode == CL_INVALID_PLATFORM);
        }

        SUBCASE("should fail if devices is set but num_entries is not") {
            auto devices = std::vector<cl_device_id>(1);
            const auto errorCode =
                clGetDeviceIDs(test::getPlatform(), CL_DEVICE_TYPE_ALL, 0,
                               devices.data(), nullptr);

            REQUIRE(errorCode == CL_INVALID_VALUE);
        }

        SUBCASE(
            "should not succeed if device with specified type is not found") {
            auto devices = std::vector<cl_device_id>(1);
            const auto errorCode =
                clGetDeviceIDs(test::getPlatform(), CL_DEVICE_TYPE_CUSTOM, 1,
                               devices.data(), nullptr);

            REQUIRE(errorCode == CL_DEVICE_NOT_FOUND);
        }

        SUBCASE("should always return device with CL_DEVICE_TYPE_ALL") {
            auto devices = std::vector<cl_device_id>(1);
            const auto errorCode =
                clGetDeviceIDs(test::getPlatform(), CL_DEVICE_TYPE_ALL, 1,
                               devices.data(), nullptr);

            REQUIRE(errorCode == CL_SUCCESS);
            REQUIRE(devices[0] == kDevice);
        }
    }

    TEST_CASE("clGetDeviceInfo") {
        SUBCASE("Hardcoded parameters") {
            SUBCASE("platform") {
                CHECK_DEVICE_PARAMETER(CL_DEVICE_PLATFORM, kPlatform);
            }

            SUBCASE("parent device") {
                CHECK_DEVICE_PARAMETER(CL_DEVICE_PARENT_DEVICE, nullptr);
            }

            SUBCASE("OpenCL version") {
                CHECK_DEVICE_STRING_PARAMETER(CL_DEVICE_OPENCL_C_VERSION,
                                              kPlatform->openClVersion);
            }

            SUBCASE("driver version") {
                CHECK_DEVICE_STRING_PARAMETER(CL_DRIVER_VERSION,
                                              kPlatform->driverVersion);
            }

            SUBCASE("device version") {
                const auto deviceVersion =
                    std::string(kPlatform->openClVersion) + " AMD (" +
                    kPlatform->driverVersion + ")";
                CHECK_DEVICE_STRING_PARAMETER(CL_DEVICE_VERSION, deviceVersion);
            }

            SUBCASE("device should be available") {
                CHECK_DEVICE_PARAMETER(CL_DEVICE_AVAILABLE, true);
            }

            SUBCASE("linker should not be available") {
                cl_bool result;
                const auto error = clGetDeviceInfo(
                    test::getDevice(), CL_DEVICE_LINKER_AVAILABLE,
                    sizeof(cl_bool), &result, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK_FALSE(result);
            }

            SUBCASE("compiler should not be available") {
                cl_bool result;
                const auto error = clGetDeviceInfo(
                    test::getDevice(), CL_DEVICE_COMPILER_AVAILABLE,
                    sizeof(cl_bool), &result, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK_FALSE(result);
            }

            SUBCASE("sub-devices are not supported") {
                cl_uint result;
                const auto error = clGetDeviceInfo(
                    test::getDevice(), CL_DEVICE_PARTITION_MAX_SUB_DEVICES,
                    sizeof(cl_uint), &result, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK(result == 0);
            }
        }

        SUBCASE("should fail with wrong parameter query") {
            char out[20];
            const auto error =
                clGetDeviceInfo(test::getDevice(), 0x0001, 20, out, nullptr);

            CHECK(error == CL_INVALID_VALUE);
        }

        SUBCASE("should not fail with AMD ext") {
            CHECK_DEVICE_STRING_PARAMETER(CL_DEVICE_BOARD_NAME_AMD,
                                          "Radeon RX 570 Series");
        }

        SUBCASE("should fail with null device") {
            const auto error =
                clGetDeviceInfo(nullptr, CL_DEVICE_NAME, 0, nullptr, nullptr);
            CHECK(error == CL_INVALID_DEVICE);
        }
    }

    TEST_CASE("clCreateSubDevices") {
        SUBCASE("clCreateSubDevices is not supported") {
            const auto error = clCreateSubDevices(test::getDevice(), nullptr, 1,
                                                  nullptr, nullptr);
            CHECK(error == CL_INVALID_DEVICE_PARTITION_COUNT);
        }
    }

    TEST_CASE("clRetainDevice") {
        SUBCASE("succeeds with root device") {
            const auto error = clRetainDevice(test::getDevice());
            CHECK(error == CL_SUCCESS);
        }
    }

    TEST_CASE("clReleaseDevice") {
        SUBCASE("succeeds with root device") {
            const auto error = clReleaseDevice(test::getDevice());
            CHECK(error == CL_SUCCESS);
        }
    }
}
