#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <common/test/doctest.h>

#include "gpu/compute_unit.h"
#include "gpu/dispatcher.h"

/**
 *  __kernel void a_plus_b(__global const int* const a,
 *                         __global const int* const b,
 *                         __global int* const c)
 * {
 *      *c = *a + *b;
 * }
 */
TEST_CASE("c = a + b") {
    auto kernArgAddr = 0;
    size_t globalWorkSize[3] = {64, 1, 1};
    const std::vector<std::string> &config = {
        ".dims  x",
        ".sgprsnum 12",
        ".vgprsnum 3",
        ".floatmode 0xc0",
        ".pgmrsrc1 0x00ac0040",
        ".pgmrsrc2 0x0000000c",
        ".dx10clamp",
        ".ieeemode",
        ".useargs",
        ".priority 0",
        ".arg _.global_offset_0, \"size_t\", long",
        ".arg _.global_offset_1, \"size_t\", long",
        ".arg _.global_offset_2, \"size_t\", long",
        ".arg _.printf_buffer, \"size_t\", void*, global, , rdonly",
        ".arg _.vqueue_pointer, \"size_t\", long",
        ".arg _.aqlwrap_pointer, \"size_t\", long",
        ".arg a, \"int*\", int*, global, const, rdonly",
        ".arg b, \"int*\", int*, global, const, rdonly",
        ".arg c, \"int*\", int*, global"
    };
    KernelConfig kernelConfig = KernelConfig(1, nullptr, globalWorkSize, globalWorkSize, config);
    kernelConfig.kernArgAddr = kernArgAddr;
    const std::vector<std::string> &instructions = {
        "/*000000000000*/ s_load_dwordx4  s[0:3], s[4:5], 0x30",
        "/*000000000008*/ s_waitcnt       lgkmcnt(0)",
        "/*00000000000c*/ s_load_dword    s0, s[0:1], 0x0",
        "/*000000000014*/ s_load_dword    s1, s[2:3], 0x0",
        "/*00000000001c*/ s_load_dwordx2  s[2:3], s[4:5], 0x40",
        "/*000000000024*/ s_waitcnt       lgkmcnt(0)",
        "/*000000000028*/ s_add_u32       s0, s1, s0",
        "/*00000000002c*/ v_mov_b32       v0, s0",
        "/*000000000030*/ v_mov_b32       v1, s2",
        "/*000000000034*/ v_mov_b32       v2, s3",
        "/*000000000038*/ flat_store_dword v[1:2], v0",
        "/*000000000040*/ s_endpgm"
    };
    KernelCode kernelCode = KernelCode(instructions);

    auto storage = Storage::get_instance();
    storage->init(128); //0 - 47 bytes contain 64-bits words: global_offset_0, global_offset_1, global_offset_2, printf_buffer,vqueue_pointer, aqlwrap_pointer
    storage->write_data(48, 0, uint64_t(72)); //write address of a
    storage->write_data(48, 8, uint64_t(76)); //write address of b
    storage->write_data(48, 16, uint64_t(80)); //write address of c
    storage->write_data(72, 0, uint32_t(3)); // a = 3
    storage->write_data(76, 0, uint32_t(10)); // b = 10

    Dispatcher dispatcher = Dispatcher(&kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }

    CHECK(storage->read_4_bytes(80, 0) == 13);
}
/**
 * __kernel __attribute__((reqd_work_group_size(64, 1, 1)))
 * void add_x_x(__global int *data, int x)
 * {
 *      uint id = get_global_id(0);
 *      data[id] = x + x;
 * }
 */
TEST_CASE("data[global_id(0)] = x + x") {
    auto kernArgAddr = 0;
    size_t globalWorkSize[3] = {1024, 1, 1};
    size_t localWorkSize[3] = {64, 1, 1};
    const std::vector<std::string> &config = {
        ".dims x",
        ".cws 64, 1, 1",
        ".sgprsnum 13",
        ".vgprsnum 3",
        ".floatmode 0xc0",
        ".pgmrsrc1 0x00ac0040",
        ".pgmrsrc2 0x0000008c",
        ".dx10clamp",
        ".ieeemode",
        ".useargs",
        ".priority 0",
        ".arg _.global_offset_0, \"size_t\", long",
        ".arg _.global_offset_1, \"size_t\", long",
        ".arg _.global_offset_2, \"size_t\", long",
        ".arg _.printf_buffer, \"size_t\", void*, global, , rdonly",
        ".arg _.vqueue_pointer, \"size_t\", long",
        ".arg _.aqlwrap_pointer, \"size_t\", long",
        ".arg data, \"int*\", int*, global, ",
        ".arg x, \"int\", int"
    };
    KernelConfig kernelConfig = KernelConfig(1, nullptr, globalWorkSize, localWorkSize, config);
    kernelConfig.kernArgAddr = kernArgAddr;
    const std::vector<std::string> &instructions = {
       "/*000000000000*/ s_load_dwordx2  s[0:1], s[4:5], 0x0",
       "/*000000000008*/ s_waitcnt       lgkmcnt(0)",
       "/*00000000000c*/ s_load_dword    s1, s[4:5], 0x38",
       "/*000000000014*/ s_load_dwordx2  s[2:3], s[4:5], 0x30",
       "/*00000000001c*/ s_lshl_b32      s4, s6, 6",
       "/*000000000020*/ s_add_u32       s0, s4, s0",
       "/*000000000024*/ v_add_u32       v0, s0, v0",
       "/*000000000028*/ v_mov_b32       v1, 0",
       "/*00000000002c*/ v_lshlrev_b64   v[0:1], 2, v[0:1]",
       "/*000000000034*/ s_waitcnt       lgkmcnt(0)",
       "/*000000000038*/ s_lshl_b32      s0, s1, 1",
       "/*00000000003c*/ v_add_u32       v0, s2, v0",
       "/*000000000040*/ v_mov_b32       v2, s3",
       "/*000000000044*/ v_addc_u32      v1, vcc, v2, v1, vcc",
       "/*000000000048*/ v_mov_b32       v2, s0",
       "/*00000000004c*/ flat_store_dword v[0:1], v2",
       "/*000000000054*/ s_endpgm"
    };
    KernelCode kernelCode = KernelCode(instructions);

    int x = 50;
    uint64_t dataAddress = 60;
    auto storage = Storage::get_instance();
    storage->init(60 + 1024); //[0:60] - args, [60:1084] - data
    storage->write_data(48, 0, uint64_t(dataAddress)); //write address of data
    storage->write_data(56, 0, uint32_t(x)); //write value of x

    Dispatcher dispatcher = Dispatcher(&kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }
    for(size_t i = 0; i < 1024; ++i) {
        CHECK(storage->read_4_bytes(dataAddress, 4 * i) == 100);
    }
}
