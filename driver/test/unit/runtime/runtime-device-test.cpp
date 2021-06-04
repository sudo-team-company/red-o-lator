#include <common/test/doctest.h>
#include <unit-test-common/test-commons.h>
#include <vector>

#include "runtime/icd/icd.h"

template <typename T>
void checkDeviceParameter(cl_device_info paramName, T expected) {
    cl_int error;
    T result;
    error = clGetDeviceInfo(test::getDevice(), paramName, sizeof(T), &result,
                            nullptr);

    CHECK(error == CL_SUCCESS);
    CHECK(result == expected);
}

void checkDeviceStringParameter(cl_device_info paramName,
                                std::string expected) {
    cl_int error;
    char result[128];
    error =
        clGetDeviceInfo(test::getDevice(), paramName, 128, &result, nullptr);

    CHECK(error == CL_SUCCESS);
    CHECK(result == expected);
}

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
            REQUIRE(deviceList[0] != nullptr);
        }

        SUBCASE("should fail if platform is null") {
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
            REQUIRE(devices[0] != nullptr);
        }

        SUBCASE("should return device if combination of flags is passed") {
            auto devices = std::vector<cl_device_id>(1);
            const auto errorCode =
                clGetDeviceIDs(test::getPlatform(),
                               CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU |
                                   CL_DEVICE_TYPE_ACCELERATOR,
                               1, devices.data(), nullptr);

            REQUIRE(errorCode == CL_SUCCESS);
            REQUIRE(devices[0] != nullptr);
        }
    }

    TEST_CASE("clGetDeviceInfo") {
        SUBCASE("Hardcoded parameters") {
            SUBCASE("platform") {
                checkDeviceParameter(CL_DEVICE_PLATFORM, test::getPlatform());
            }

            SUBCASE("parent device") {
                checkDeviceParameter<cl_device_id>(CL_DEVICE_PARENT_DEVICE,
                                                   nullptr);
            }

            SUBCASE("OpenCL version") {
                checkDeviceStringParameter(CL_DEVICE_OPENCL_C_VERSION,
                                           test::getPlatform()->openClVersion);
            }

            SUBCASE("driver version") {
                checkDeviceStringParameter(CL_DRIVER_VERSION,
                                           test::getPlatform()->driverVersion);
            }

            SUBCASE("device version") {
                const auto deviceVersion =
                    std::string(test::getPlatform()->openClVersion) +
                    " AMD-APP (" + test::getPlatform()->driverVersion + ")";
                checkDeviceStringParameter(CL_DEVICE_VERSION, deviceVersion);
            }

            SUBCASE("device should be available") {
                checkDeviceParameter(CL_DEVICE_AVAILABLE, CL_TRUE);
            }

            SUBCASE("linker should not be available") {
                checkDeviceParameter(CL_DEVICE_LINKER_AVAILABLE, CL_FALSE);
            }

            SUBCASE("compiler should not be available") {
                checkDeviceParameter(CL_DEVICE_COMPILER_AVAILABLE, CL_FALSE);
            }

            SUBCASE("sub-devices are not supported") {
                checkDeviceParameter<cl_uint>(
                    CL_DEVICE_PARTITION_MAX_SUB_DEVICES, 0);
            }

            SUBCASE("image is not supported") {
                checkDeviceParameter<cl_uint>(CL_DEVICE_IMAGE_SUPPORT,
                                              CL_FALSE);
            }

            SUBCASE("host and device memory is unified") {
                checkDeviceParameter<cl_uint>(CL_DEVICE_HOST_UNIFIED_MEMORY,
                                              CL_TRUE);
            }

            SUBCASE("error resolution is not supported") {
                checkDeviceParameter<cl_uint>(
                    CL_DEVICE_ERROR_CORRECTION_SUPPORT, CL_FALSE);
            }
        }

        SUBCASE("should get array of items correctly") {
            size_t out[3];
            const auto error = clGetDeviceInfo(
                test::getDevice(), CL_DEVICE_MAX_WORK_ITEM_SIZES,
                3 * sizeof(size_t), out, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(out[0] == 1024);
            CHECK(out[1] == 1024);
            CHECK(out[2] == 1024);
        }

        SUBCASE("should fail with wrong parameter query") {
            char out[20];
            const auto error =
                clGetDeviceInfo(test::getDevice(), 0x0001, 20, out, nullptr);

            CHECK(error == CL_INVALID_VALUE);
        }

        SUBCASE("should not fail with AMD ext") {
            checkDeviceStringParameter(CL_DEVICE_BOARD_NAME_AMD,
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
