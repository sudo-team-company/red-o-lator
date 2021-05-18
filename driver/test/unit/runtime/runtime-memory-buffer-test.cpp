#include <common/test/doctest.h>

#include <cstring>
#include <vector>

#include "runtime/icd/icd.h"
#include "unit-test-common/test-commons.h"

void checkMutuallyExclusiveness(std::vector<cl_mem_flags> flags,
                                bool useHostPtr) {
    for (int i = 0; i < flags.size(); ++i) {
        for (int j = 0; j < flags.size(); ++j) {
            if (i != j) {
                if (useHostPtr) {
                    unsigned char hostPtr[2];
                    const auto [_, error] = test::createBufferSafe(
                        flags[i] | flags[j], 16, &hostPtr);
                    CHECK(error == CL_INVALID_VALUE);
                } else {
                    const auto [_, error] =
                        test::createBufferSafe(flags[i] | flags[j]);
                    CHECK(error == CL_INVALID_VALUE);
                }
            }
        }
    }
}

TEST_SUITE("Memory buffer API") {
    TEST_CASE("clCreateBuffer") {
        SUBCASE("buffer can be created with reference count equal to 1") {
            const auto buffer = test::createBuffer();
            CHECK(buffer->referenceCount == 1);
        }

        SUBCASE("retains context") {
            auto context = test::getContext();
            const auto initialRefCount = context->referenceCount;

            cl_int error;
            cl_mem buffer = clCreateBuffer(context, 0, 16, nullptr, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(buffer->context->referenceCount == initialRefCount + 1);
        }

        SUBCASE("increments device memory size") {
            auto context = test::getContext();
            const auto initialUsedGlobalMemorySize =
                context->device->usedGlobalMemory;
            const auto bufferSize = 16;

            cl_int error;
            cl_mem buffer =
                clCreateBuffer(context, 0, bufferSize, nullptr, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(context->device->usedGlobalMemory ==
                  initialUsedGlobalMemorySize + bufferSize);
        }

        SUBCASE("fail if is not enough device memory") {
            const auto context = test::getContext();
            const auto deviceMemorySize = context->device->globalMemorySize;

            cl_int error;
            cl_mem buffer = clCreateBuffer(context, 0, deviceMemorySize + 1,
                                           nullptr, &error);
            CHECK(error == CL_MEM_OBJECT_ALLOCATION_FAILURE);
        }

        SUBCASE("fail if context is null") {
            cl_int error;
            clCreateBuffer(nullptr, 0, 0, nullptr, &error);
            CHECK(error == CL_INVALID_CONTEXT);
        }

        SUBCASE("fail if buffer size is 0") {
            const auto [buffer, error] = test::createBufferSafe(0, 0);
            CHECK(error == CL_INVALID_BUFFER_SIZE);
        }

        SUBCASE("kernel memory access flags") {
            SUBCASE("read-only access if CL_MEM_READ_ONLY") {
                const auto buffer = test::createBuffer(CL_MEM_READ_ONLY);

                CHECK(buffer->kernelCanRead == true);
                CHECK(buffer->kernelCanWrite == false);
            }

            SUBCASE("write-only access if CL_MEM_WRITE_ONLY") {
                const auto buffer = test::createBuffer(CL_MEM_WRITE_ONLY);

                CHECK(buffer->kernelCanRead == false);
                CHECK(buffer->kernelCanWrite == true);
            }

            SUBCASE("RW access if CL_MEM_READ_WRITE") {
                const auto buffer = test::createBuffer(CL_MEM_READ_WRITE);

                CHECK(buffer->kernelCanRead == true);
                CHECK(buffer->kernelCanWrite == true);
            }

            SUBCASE("mutually exclusiveness") {
                checkMutuallyExclusiveness(
                    {CL_MEM_WRITE_ONLY, CL_MEM_READ_ONLY, CL_MEM_READ_WRITE},
                    false);
            }
        }

        SUBCASE("host memory access flags") {
            SUBCASE("read-write access if no flags specified") {
                const auto buffer = test::createBuffer();

                CHECK(buffer->hostCanRead == true);
                CHECK(buffer->hostCanWrite == true);
            }

            SUBCASE("read-only access if CL_MEM_HOST_READ_ONLY") {
                const auto buffer = test::createBuffer(CL_MEM_HOST_READ_ONLY);

                CHECK(buffer->hostCanRead == true);
                CHECK(buffer->hostCanWrite == false);
            }

            SUBCASE("write-only access if CL_MEM_HOST_WRITE_ONLY") {
                const auto buffer = test::createBuffer(CL_MEM_HOST_WRITE_ONLY);

                CHECK(buffer->hostCanRead == false);
                CHECK(buffer->hostCanWrite == true);
            }

            SUBCASE("no access if CL_MEM_HOST_NO_ACCESS") {
                const auto buffer = test::createBuffer(CL_MEM_HOST_NO_ACCESS);

                CHECK(buffer->hostCanRead == false);
                CHECK(buffer->hostCanWrite == false);
            }

            SUBCASE("mutually exclusiveness") {
                checkMutuallyExclusiveness(
                    {CL_MEM_HOST_READ_ONLY, CL_MEM_HOST_WRITE_ONLY,
                     CL_MEM_HOST_NO_ACCESS},
                    false);
            }
        }

        SUBCASE("host ptr parameters") {
            SUBCASE("host_ptr should present if host ptr flags are specified") {
                SUBCASE("CL_MEM_USE_HOST_PTR") {
                    const auto [buffer, error] =
                        test::createBufferSafe(CL_MEM_USE_HOST_PTR);
                    CHECK(error == CL_INVALID_HOST_PTR);
                }

                SUBCASE("CL_MEM_COPY_HOST_PTR") {
                    const auto [buffer, error] =
                        test::createBufferSafe(CL_MEM_COPY_HOST_PTR);
                    CHECK(error == CL_INVALID_HOST_PTR);
                }
            }

            SUBCASE("flags should present if host_ptr is specified") {
                unsigned char hostPtr[2];
                const auto [buffer, error] =
                    test::createBufferSafe(0, 16, &hostPtr);
                CHECK(error == CL_INVALID_HOST_PTR);
            }

            SUBCASE("mutually exclusiveness") {
                checkMutuallyExclusiveness(
                    {CL_MEM_USE_HOST_PTR, CL_MEM_ALLOC_HOST_PTR}, true);
                checkMutuallyExclusiveness(
                    {CL_MEM_USE_HOST_PTR, CL_MEM_COPY_HOST_PTR}, true);
            }
        }
    }

    TEST_CASE("clEnqueue{Read/Write}Buffer") {
        
    }
}
