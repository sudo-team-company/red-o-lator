#include <CL/cl.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <common/test/doctest.h>

std::vector<unsigned char> readBinaryFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary);
    return std::vector<unsigned char>(std::istreambuf_iterator<char>(input),
                                      {});
}

TEST_CASE("a_plus_b") {
    cl_uint errorCode;

    std::vector<cl::Platform> platforms;
    REQUIRE_NOTHROW(cl::Platform::get(&platforms));
    const auto platform = platforms[0];
    INFO(platform.getInfo<CL_PLATFORM_VENDOR>());

    std::vector<cl::Device> devices;
    REQUIRE_NOTHROW(platform.getDevices(CL_DEVICE_TYPE_GPU, &devices));
    const auto device = devices[0];
    INFO(device.getInfo<CL_DEVICE_NAME>());

    cl::Context context;
    REQUIRE_NOTHROW(context = cl::Context(device));

    cl::CommandQueue queue;
    REQUIRE_NOTHROW(queue = cl::CommandQueue(context));

    const auto binary = readBinaryFile("../resources/kernels/a_plus_b.bin");
    REQUIRE_FALSE(binary.empty());
    cl::Program::Binaries binaryObj = {{binary.data(), binary.size()}};

    cl::Program program;
    REQUIRE_NOTHROW(program = cl::Program(context, devices, binaryObj));
    REQUIRE_NOTHROW(program.build());

    const size_t arraySize = 100000;
    const size_t arraySizeMem = arraySize * sizeof(cl_uint);

    std::vector<cl_uint> inputA;
    inputA.assign(arraySize, 100);
    cl_mem inputABuffer;
    REQUIRE_NOTHROW(inputABuffer = clCreateBuffer(
                        context(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                        arraySizeMem, inputA.data(), nullptr));

    std::vector<cl_uint> inputB;
    inputB.assign(arraySize, 200);
    cl_mem inputBBuffer;
    REQUIRE_NOTHROW(inputBBuffer = clCreateBuffer(
                        context(), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                        arraySizeMem, inputB.data(), nullptr));

    std::vector<cl_uint> output;
    output.assign(arraySize, 0);
    cl_mem outputBuffer;
    REQUIRE_NOTHROW(outputBuffer = clCreateBuffer(context(), CL_MEM_WRITE_ONLY,
                                         arraySizeMem, nullptr, nullptr));

    cl::Kernel kernel;
    REQUIRE_NOTHROW(kernel = cl::Kernel(program, "a_plus_b"));
    REQUIRE_NOTHROW(kernel.setArg(0, inputABuffer));
    REQUIRE_NOTHROW(kernel.setArg(1, inputBBuffer));
    REQUIRE_NOTHROW(kernel.setArg(2, outputBuffer));
//    REQUIRE_NOTHROW(queue.enqueueNDRangeKernel(kernel, 0, 0));
}
