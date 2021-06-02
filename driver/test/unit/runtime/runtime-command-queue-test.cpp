#include <common/test/doctest.h>

#include "runtime/icd/icd.h"
#include "unit-test-common/test-commons.h"

TEST_SUITE("Command queue API") {
    TEST_CASE("clCreateCommandQueue") {
        SUBCASE("should create command queue from context") {
            const auto context = test::getContext();
            cl_int error;
            const auto queue =
                clCreateCommandQueue(context, context->device, 0, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(queue != nullptr);
        }

        SUBCASE("out of order supported") {
            const auto context = test::getContext();
            cl_int error;
            const auto queue = clCreateCommandQueue(
                context, context->device,
                CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(queue != nullptr);
        }

        SUBCASE("profiling supported") {
            const auto context = test::getContext();
            cl_int error;
            const auto queue = clCreateCommandQueue(
                context, context->device, CL_QUEUE_PROFILING_ENABLE, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(queue != nullptr);
        }

        SUBCASE("reference count should be 1 after creation") {
            CHECK(test::getCommandQueue()->referenceCount == 1);
        }

        SUBCASE("should retain context after created") {
            const auto context = test::getContext();
            const auto initialRefCount = context->referenceCount;

            cl_int error;
            const auto queue =
                clCreateCommandQueue(context, context->device, 0, &error);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->context->referenceCount == initialRefCount + 1);
        }

        SUBCASE("should fail with null context") {
            cl_int error;
            const auto queue = clCreateCommandQueue(
                nullptr, test::getDevice(), CL_QUEUE_PROFILING_ENABLE, &error);

            CHECK(error == CL_INVALID_CONTEXT);
            CHECK(queue == nullptr);
        }

        SUBCASE("should fail with null device") {
            cl_int error;
            const auto queue = clCreateCommandQueue(
                test::getContext(), nullptr, CL_QUEUE_PROFILING_ENABLE, &error);

            CHECK(error == CL_INVALID_DEVICE);
            CHECK(queue == nullptr);
        }
    }

    TEST_CASE("clFinish") {
        SUBCASE("should flush queue") {
            const auto queue = test::getCommandQueue();
            queue->enqueue(test::getDummyCommand(queue));
            queue->enqueue(test::getDummyCommand(queue));

            CHECK(queue->size() == 2);

            clFinish(queue);

            CHECK(queue->size() == 0);
        }

        SUBCASE("should fail with null command queue") {
            CHECK(clFinish(nullptr) == CL_INVALID_COMMAND_QUEUE);
        }
    }

    TEST_CASE("clFlush") {
        SUBCASE("calls clFinish") {
            CHECK(clFlush(test::getCommandQueue()) == CL_SUCCESS);
        }
    }

    TEST_CASE("clRetainCommandQueue") {
        SUBCASE("increments command queue ref count") {
            auto queue = test::getCommandQueue();
            const auto initialRefCount = queue->referenceCount;
            const auto error = clRetainCommandQueue(queue);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->referenceCount == initialRefCount + 1);
        }

        SUBCASE("fails if queue is null") {
            CHECK(clRetainCommandQueue(nullptr) == CL_INVALID_COMMAND_QUEUE);
        }
    }

    TEST_CASE("clReleaseCommandQueue") {
        SUBCASE("should decrement ref count") {
            auto queue = test::getCommandQueue();

            clRetainCommandQueue(queue);

            const auto initialRefCount = queue->referenceCount;
            const auto error = clReleaseCommandQueue(queue);

            CHECK(error == CL_SUCCESS);
            CHECK(queue->referenceCount == initialRefCount - 1);
        }

        SUBCASE("should release context upon deletion") {
            auto queue = test::getCommandQueue();
            auto context = queue->context;
            const auto contextRefCount = context->referenceCount;
            const auto error = clReleaseCommandQueue(queue);

            CHECK(error == CL_SUCCESS);

            // check if context reference counter has been decremented meaning
            // queue destructor has been called
            CHECK(context->referenceCount == contextRefCount - 1);
        }

        SUBCASE("flushes queue if it is not empty") {
            auto queue = test::getCommandQueue();
            auto context = queue->context;

            const auto cmd = test::getDummyCommand(queue);
            const auto event = cmd->requireEvent();

            const auto contextRefCount = context->referenceCount;

            queue->enqueue(cmd);

            const auto error = clReleaseCommandQueue(queue);

            CHECK(error == CL_SUCCESS);

            // check if context reference counter has been decremented meaning
            // queue destructor has been called
            CHECK(context->referenceCount == contextRefCount - 1);
            CHECK(event->referenceCount == 1);
        }
    }

    TEST_CASE("clGetCommandQueueInfo") {
        SUBCASE("should fail with null queue") {
            cl_int error = clGetCommandQueueInfo(nullptr, CL_QUEUE_CONTEXT, 0,
                                                 nullptr, nullptr);
            CHECK(error == CL_INVALID_COMMAND_QUEUE);
        }

        SUBCASE("should get context") {
            auto queue = test::getCommandQueue();
            cl_context context;
            cl_int error = clGetCommandQueueInfo(
                queue, CL_QUEUE_CONTEXT, sizeof(cl_context), &context, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(context == queue->context);
        }

        SUBCASE("should get device") {
            auto queue = test::getCommandQueue();
            cl_device_id device;
            cl_int error = clGetCommandQueueInfo(
                queue, CL_QUEUE_DEVICE, sizeof(cl_device_id), &device, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(device == queue->context->device);
        }

        SUBCASE("should get ref count") {
            auto queue = test::getCommandQueue();
            cl_uint refCount;
            cl_int error =
                clGetCommandQueueInfo(queue, CL_QUEUE_REFERENCE_COUNT,
                                      sizeof(cl_uint), &refCount, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(refCount == queue->referenceCount);
        }

        SUBCASE("should get properties") {
            auto queue = test::getCommandQueue();
            cl_command_queue_properties props;
            cl_int error = clGetCommandQueueInfo(
                queue, CL_QUEUE_PROPERTIES, sizeof(cl_command_queue_properties),
                &props, nullptr);

            CHECK(error == CL_SUCCESS);
            CHECK(props == queue->properties);
        }
    }
}

TEST_SUITE("CLCommandQueue") {
    TEST_CASE("enqueue") {
        SUBCASE("should increment queue size") {
            auto queue = test::getCommandQueue();

            CHECK(queue->size() == 0);

            queue->enqueue(test::getDummyCommand(queue));
            queue->enqueue(test::getDummyCommand(queue));
            queue->enqueue(test::getDummyCommand(queue));

            CHECK(queue->size() == 3);
        }
    }

    TEST_CASE("flush") {
        SUBCASE("should clear command queue") {
            auto queue = test::getCommandQueue();
            queue->enqueue(test::getDummyCommand(queue));
            queue->enqueue(test::getDummyCommand(queue));
            queue->enqueue(test::getDummyCommand(queue));

            CHECK(queue->size() == 3);

            queue->flush();

            CHECK(queue->size() == 0);
        }
    }
}
