#include <common/test/doctest.h>
#include <vector>

#include "runtime/icd/icd.h"
#include "unit-test-common/test-commons.h"

template <typename T>
void checkParameter(cl_context context, cl_context_info paramName, T expected) {
    T result;
    cl_int error =
        clGetContextInfo(context, paramName, sizeof(T), &result, nullptr);

    CHECK(error == CL_SUCCESS);
    CHECK(result == expected);
}

TEST_SUITE("Context API") {
    TEST_CASE("clCreateContext") {
        SUBCASE("should create context from device") {
            cl_int error;
            const std::vector<cl_device_id> devices = {test::getDevice()};
            cl_context context = clCreateContext(nullptr, 1, devices.data(),
                                                 nullptr, nullptr, &error);
            CHECK(error == CL_SUCCESS);
            CHECK(context != nullptr);
            CHECK(context->device == test::getDevice());
        }

        SUBCASE("context reference count should be 1 after created") {
            const auto context = test::getContext();
            CHECK(context->referenceCount == 1);
        }

        SUBCASE("should fail with null devices") {
            cl_int error;
            clCreateContext(nullptr, 1, nullptr, nullptr, nullptr, &error);
            CHECK(error == CL_INVALID_VALUE);
        }

        SUBCASE("should fail with 0 num_devices") {
            cl_int error;

            const std::vector<cl_device_id> devices = {test::getDevice()};
            clCreateContext(nullptr, 0, devices.data(),
                                                 nullptr, nullptr, &error);
            CHECK(error == CL_INVALID_VALUE);
        }

        SUBCASE("should fail with invalid callback parameters") {
            cl_int error;
            const std::vector<cl_device_id> devices = {test::getDevice()};
            clCreateContext(nullptr, 1, devices.data(),
                                                 nullptr, nullptr, &error);
            CHECK(error == CL_SUCCESS);
        }
    }

    TEST_CASE("clCreateContextFromType") {
        SUBCASE("can create context from type") {
            cl_int error;
            cl_context context = clCreateContextFromType(
                nullptr, CL_DEVICE_TYPE_GPU, nullptr, nullptr, &error);
            CHECK(error == CL_SUCCESS);
            CHECK(context->device == test::getDevice());
        }
    }

    TEST_CASE("clRetainContext") {
        SUBCASE("should increment context reference count") {
            auto context = test::getContext();
            const auto refCount = context->referenceCount;
            const auto error = clRetainContext(context);

            CHECK(error == CL_SUCCESS);
            CHECK(context->referenceCount == refCount + 1);
        }

        SUBCASE("should fail if context is null") {
            const auto error = clRetainContext(nullptr);
            CHECK(error == CL_INVALID_CONTEXT);
        }
    }

    TEST_CASE("clReleaseContext") {
        SUBCASE("should decrement context reference count") {
            auto context = test::getContext();
            const auto refCount = context->referenceCount;

            const auto error = clReleaseContext(context);

            CHECK(error == CL_SUCCESS);
            CHECK(context->referenceCount == refCount - 1);
        }

        SUBCASE("should fail if context is null") {
            const auto error = clReleaseContext(nullptr);
            CHECK(error == CL_INVALID_CONTEXT);
        }
    }

    TEST_CASE("clGetContextInfo") {
        SUBCASE("should fail if context is null") {
            const auto error = clGetContextInfo(
                nullptr, CL_CONTEXT_REFERENCE_COUNT, 0, nullptr, nullptr);

            CHECK(error == CL_INVALID_CONTEXT);
        }

        SUBCASE("should return reference count") {
            const auto context = test::getContext();
            checkParameter(context, CL_CONTEXT_REFERENCE_COUNT, 1);
        }

        SUBCASE("num_devices should always be 1") {
            const auto context = test::getContext();
            checkParameter(context, CL_CONTEXT_NUM_DEVICES, 1);
        }

        SUBCASE("should return correct device") {
            const auto context = test::getContext();
            cl_device_id expected = context->device;
            checkParameter<cl_device_id>(context, CL_CONTEXT_DEVICES, expected);
        }

        SUBCASE("properties are not supported yet") {
            const auto context = test::getContext();
            cl_context_properties* actual;
            cl_int error = clGetContextInfo(context, CL_CONTEXT_PROPERTIES,
                                            10 * sizeof(cl_context_properties),
                                            &actual, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(actual == nullptr);
        }
    }
}