#include <common/test/doctest.h>

#include <common/utils/vector-utils.hpp>
#include <vector>

#include "runtime/icd/icd.h"
#include "unit-test-common/test-commons.h"

TEST_SUITE("Memory API") {
    TEST_CASE("clRetainMemObject") {
        SUBCASE("increments mem object reference count") {
            const auto buffer = test::createBuffer();
            const auto initRefCount = buffer->referenceCount;
            clRetainMemObject(buffer);
            CHECK(buffer->referenceCount == initRefCount + 1);
        }

        SUBCASE("fails with null mem object") {
            CHECK(clRetainMemObject(nullptr) == CL_INVALID_MEM_OBJECT);
        }
    }

    TEST_CASE("clReleaseMemObject") {
        SUBCASE("decrements mem object reference count") {
            const auto buffer = test::createBuffer();
            clRetainMemObject(buffer);
            const auto initRefCount = buffer->referenceCount;

            clReleaseMemObject(buffer);

            CHECK(buffer->referenceCount == initRefCount - 1);
        }

        SUBCASE("should release context when ref count reaches zero") {
            const auto buffer = test::createBuffer();
            const auto context = buffer->context;
            const auto initRefCount = context->referenceCount;

            clReleaseMemObject(buffer);

            CHECK(context->referenceCount == initRefCount - 1);
        }

        SUBCASE("should release memory when ref count reaches zero") {
            const auto bufferSize = 16;
            const auto buffer = test::createBuffer(0, bufferSize);
            const auto device = buffer->context->device;
            const auto initUsedMemory = device->usedGlobalMemory;

            clReleaseMemObject(buffer);

            CHECK(device->usedGlobalMemory == initUsedMemory - bufferSize);
        }

        SUBCASE("fail if mem object is null") {
            CHECK(clReleaseMemObject(nullptr) == CL_INVALID_MEM_OBJECT);
        }
    }

    TEST_CASE("clSetMemObjectDestructorCallback") {
        SUBCASE(
            "should register destructor callbacks to be called in "
            "LIFO order after destruction") {
            const auto buffer = test::createBuffer();

            std::vector<std::string> callbackOrder;
            auto makeUserData = [&](const std::string& value) {
                return std::make_pair(&callbackOrder, value);
            };

            CLMemDestructorCallbackFunction callback = [](cl_mem memObj,
                                                          void* userData) {
                auto* data =
                    (std::pair<std::vector<std::string>*, std::string>*)
                        userData;

                data->first->push_back(data->second);
            };

            auto userData1 = makeUserData("1");
            auto userData2 = makeUserData("2");
            auto userData3 = makeUserData("3");

            clSetMemObjectDestructorCallback(buffer, callback, &userData1);
            clSetMemObjectDestructorCallback(buffer, callback, &userData2);
            clSetMemObjectDestructorCallback(buffer, callback, &userData3);

            clReleaseMemObject(buffer);

            CHECK(utils::joinToString(callbackOrder, " ") == "3 2 1");
        }
    }
}
