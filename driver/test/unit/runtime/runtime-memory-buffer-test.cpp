#include <common/test/doctest.h>

#include <algorithm>
#include <common/utils/common.hpp>
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

            SUBCASE(
                "CL_MEM_COPY_HOST_PTR: memory from host_ptr is copied to "
                "buffer") {
                const auto size = 10;
                const auto sizeBytes = 10 * sizeof(cl_uint);

                std::vector<cl_uint> data{};
                test::fillVector(size, data);

                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_COPY_HOST_PTR,
                                                       sizeBytes, data.data());

                auto out = std::vector<cl_uint>(size);
                const auto error =
                    clEnqueueReadBuffer(queue, buffer, true, 0, sizeBytes,
                                        out.data(), 0, nullptr, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK(utils::joinToString<cl_uint>(out, " ") ==
                      utils::joinToString<cl_uint>(data, " "));
            }

            SUBCASE(
                "CL_MEM_COPY_HOST_PTR: writing to hostPtr does not change "
                "buffer") {
                const auto size = 10;
                const auto sizeBytes = size * sizeof(cl_uint);

                std::vector<cl_uint> data{};
                test::fillVector(size, data);

                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_COPY_HOST_PTR,
                                                       sizeBytes, data.data());

                const auto expected = utils::joinToString<cl_uint>(data, " ");

                data[0] = 20;

                auto out = std::vector<cl_uint>(size);
                const auto error =
                    clEnqueueReadBuffer(queue, buffer, true, 0, sizeBytes,
                                        out.data(), 0, nullptr, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK(utils::joinToString<cl_uint>(out, " ") == expected);
            }

            SUBCASE(
                "CL_MEM_USE_HOST_PTR: writing to hostPtr from host changes "
                "buffer") {
                const auto size = 10;
                const auto sizeBytes = size * sizeof(cl_uint);

                std::vector<cl_uint> data{};
                test::fillVector(size, data);

                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_USE_HOST_PTR,
                                                       sizeBytes, data.data());

                data[0] = 20;

                auto out = std::vector<cl_uint>(size);
                const auto error =
                    clEnqueueReadBuffer(queue, buffer, true, 0, sizeBytes,
                                        out.data(), 0, nullptr, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK(utils::joinToString<cl_uint>(out, " ") ==
                      utils::joinToString<cl_uint>(data, " "));
            }

            SUBCASE(
                "CL_MEM_USE_HOST_PTR: writing to buffer from host changes "
                "hostPtr") {
                const auto size = 10;
                const auto sizeBytes = size * sizeof(cl_uint);

                std::vector<cl_uint> data{};
                test::fillVector(size, data);

                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_USE_HOST_PTR,
                                                       sizeBytes, data.data());

                std::vector<cl_uint> toWrite{20};
                const auto error = clEnqueueWriteBuffer(
                    queue, buffer, true, 0, sizeof(cl_uint), toWrite.data(), 0,
                    nullptr, nullptr);

                CHECK(error == CL_SUCCESS);
                CHECK(data[0] == 20);
            }
        }
    }

    TEST_CASE("clEnqueueReadBuffer") {
        SUBCASE("content without flags is initialized to zero") {
            const auto size = 8;
            const auto sizeBytes = size * sizeof(cl_uint);

            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer(0, sizeBytes);

            auto out = std::vector<cl_uint>(size);
            out.assign(size, 1);
            const auto error =
                clEnqueueReadBuffer(queue, buffer, true, 0, sizeBytes,
                                    out.data(), 0, nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(std::all_of(out.begin(), out.end(), [](auto value) {
                return value == 0;
            }));
        }

        SUBCASE("should flush queue only if blocking_read is true") {
            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer();

            cl_int error = 0;
            std::byte data[1];
            error = clEnqueueReadBuffer(queue, buffer, false, 0, 1, data, 0,
                                        nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->size() == 1);

            error = clEnqueueReadBuffer(queue, buffer, true, 0, 1, data, 0,
                                        nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->size() == 0);
        }

        SUBCASE("should read memory block with offset") {
            const auto totalSize = 10;
            const auto totalSizeBytes = totalSize * sizeof(cl_uint);

            std::vector<cl_uint> data{};
            test::fillVector(totalSize, data);

            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer(CL_MEM_COPY_HOST_PTR,
                                                   totalSizeBytes, data.data());

            const auto offset = 3;
            const auto offsetBytes = offset * sizeof(cl_uint);
            const auto offsetSize = 3;
            const auto offsetSizeBytes = offsetSize * sizeof(cl_uint);

            std::vector<cl_uint> out(offsetSize);
            const auto error = clEnqueueReadBuffer(
                queue, buffer, true, offsetBytes, offsetSizeBytes, out.data(),
                0, nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(utils::joinToString<cl_uint>(out, " ") == "3 4 5");
        }
    }

    TEST_CASE("clEnqueueWriteBuffer") {
        SUBCASE("should flush queue only if blocking_read is true") {
            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer();

            cl_int error = 0;
            std::byte data[1];
            error = clEnqueueWriteBuffer(queue, buffer, false, 0, 1, data, 0,
                                         nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->size() == 1);

            error = clEnqueueWriteBuffer(queue, buffer, true, 0, 1, data, 0,
                                         nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->size() == 0);
        }

        SUBCASE("should write to memory block with offset") {
            const auto totalSize = 6;
            const auto totalSizeBytes = totalSize * sizeof(cl_uint);

            std::vector<cl_uint> data{};
            test::fillVector(totalSize, data);

            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer(CL_MEM_USE_HOST_PTR,
                                                   totalSizeBytes, data.data());

            const auto offset = 3;
            const auto offsetBytes = offset * sizeof(cl_uint);
            const auto offsetSize = 3;
            const auto offsetSizeBytes = offsetSize * sizeof(cl_uint);

            std::vector<cl_uint> toWrite(offsetSize);
            toWrite.assign(offsetSize, 20);
            const auto error = clEnqueueWriteBuffer(
                queue, buffer, true, offsetBytes, offsetSizeBytes,
                toWrite.data(), 0, nullptr, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(utils::joinToString<cl_uint>(data, " ") == "0 1 2 20 20 20");
        }
    }

    TEST_CASE("clEnqueue{Read/Write}Buffer") {
        SUBCASE("should fail if ptr or size is not set") {
            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer();

            cl_int error;

            cl_uint data[1];
            error = clEnqueueReadBuffer(queue, buffer, true, 0, 0, data, 0,
                                        nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);

            error = clEnqueueWriteBuffer(queue, buffer, true, 0, 0, data, 0,
                                         nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);

            error = clEnqueueReadBuffer(queue, buffer, true, 0, 10, nullptr, 0,
                                        nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);

            error = clEnqueueReadBuffer(queue, buffer, true, 0, 10, nullptr, 0,
                                        nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);
        }

        SUBCASE(
            "should fail if requested size if more than buffer size with "
            "reвpect to offset") {
            const auto size = 16;
            const auto sizeBytes = 16 * sizeof(cl_uint);
            auto data = new cl_uint[size];

            const auto queue = test::getCommandQueue();
            const auto buffer = test::createBuffer(0, sizeBytes);

            cl_int error;

            error = clEnqueueReadBuffer(queue, buffer, true, 0, sizeBytes + 4,
                                        data, 0, nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);

            error = clEnqueueWriteBuffer(queue, buffer, true, 0, sizeBytes + 4,
                                         data, 0, nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);

            error = clEnqueueReadBuffer(queue, buffer, true, 4, sizeBytes, data,
                                        0, nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);

            error = clEnqueueWriteBuffer(queue, buffer, true, 4, sizeBytes,
                                         data, 0, nullptr, nullptr);
            CHECK(error == CL_INVALID_VALUE);
        }

        SUBCASE("host access") {
            SUBCASE("host can't read from write-only memory") {
                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_HOST_WRITE_ONLY);

                cl_uint data[1];
                const auto error = clEnqueueReadBuffer(
                    queue, buffer, true, 0, 1, data, 0, nullptr, nullptr);
                CHECK(error == CL_INVALID_OPERATION);
            }

            SUBCASE("host can't write to read-only memory") {
                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_HOST_READ_ONLY);

                cl_uint data[1];
                const auto error = clEnqueueWriteBuffer(
                    queue, buffer, true, 0, 1, data, 0, nullptr, nullptr);
                CHECK(error == CL_INVALID_OPERATION);
            }

            SUBCASE("host can't read or write to no-access memory") {
                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer(CL_MEM_HOST_NO_ACCESS);

                cl_int error;
                cl_uint data[1];

                error = clEnqueueReadBuffer(queue, buffer, true, 0, 1, data, 0,
                                            nullptr, nullptr);
                CHECK(error == CL_INVALID_OPERATION);

                error = clEnqueueWriteBuffer(queue, buffer, true, 0, 1, data, 0,
                                             nullptr, nullptr);
                CHECK(error == CL_INVALID_OPERATION);
            }

            SUBCASE("host can read and write to no-host-flags memory") {
                const auto queue = test::getCommandQueue();
                const auto buffer = test::createBuffer();

                cl_int error;

                const auto size = 1;
                const auto sizeBytes = size * sizeof(cl_uint);
                auto data = new cl_uint[size];

                error = clEnqueueReadBuffer(queue, buffer, true, 0, sizeBytes,
                                            data, 0, nullptr, nullptr);
                CHECK(error == CL_SUCCESS);

                error = clEnqueueWriteBuffer(queue, buffer, true, 0, sizeBytes,
                                             data, 0, nullptr, nullptr);
                CHECK(error == CL_SUCCESS);
            }
        }
    }
}
