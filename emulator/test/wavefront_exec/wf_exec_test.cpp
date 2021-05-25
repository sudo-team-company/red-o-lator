//
// Created by Diana Kudaiberdieva
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../../gpu/compute_unit.h"
#include "../../gpu/dispatcher.h"
#include "../doctest.h"

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
    // initialize storage
    auto storage = Storage::get_instance();
    storage->init(128); //0 - 47 bytes contain 64-bits words: global_offset_0, global_offset_1, global_offset_2, printf_buffer,vqueue_pointer, aqlwrap_pointer
    storage->write_data(48, 0, uint64_t(72)); //write address of a
    storage->write_data(48, 8, uint64_t(76)); //write address of b
    storage->write_data(48, 16, uint64_t(80)); //write address of c
    storage->write_data(72, 0, uint32_t(3)); // a = 3
    storage->write_data(76, 0, uint32_t(7)); // b = 10

    KernelConfig kernelConfig = KernelConfig();
    kernelConfig.kernArgAddr = kernArgAddr;
    kernelConfig.vgprsnum = 3;
    kernelConfig.sgprsnum = 12;
    kernelConfig.floatmode = 0xc0;
    kernelConfig.pgmrsrc1 = 0x00ac0040;
    kernelConfig.pgmrsrc2 = 0x0000000c;
    kernelConfig.ndRangeSizeX = 1;
    kernelConfig.ndRangeSizeY = 1;
    kernelConfig.ndRangeSizeZ = 1;
    kernelConfig.wgSizeX = 1;
    kernelConfig.wgSizeY = 1;
    kernelConfig.wgSizeZ = 1;

    KernelCode kernelCode = KernelCode();
    kernelCode.add_instr(0, "s_load_dwordx4", {"s[0:3]", "s[4:5]", "0x30"});
    kernelCode.add_instr(0x8, "s_waitcnt", {"lgkmcnt(0)"});
    kernelCode.add_instr(0xc, "s_load_dword", {"s0", "s[0:1]", "0x0"});
    kernelCode.add_instr(0x14, "s_load_dword", {"s1", "s[2:3]", "0x0"});
    kernelCode.add_instr(0x1c, "s_load_dwordx2", {"s[2:3]", "s[4:5]", "0x40"});
    kernelCode.add_instr(0x24, "s_waitcnt", {"lgkmcnt(0)"});
    kernelCode.add_instr(0x28, "s_add_u32", {"s0", "s1", "s0"});
    kernelCode.add_instr(0x2c, "v_mov_b32", {"v0", "s0"});
    kernelCode.add_instr(0x30, "v_mov_b32", {"v1", "s2"});
    kernelCode.add_instr(0x34, "v_mov_b32", {"v2", "s3"});
    kernelCode.add_instr(0x38, "flat_store_dword", {"v[1:2]", "v0"});
    kernelCode.add_instr(0x40, "s_endpgm", {});

    Dispatcher dispatcher = Dispatcher(kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }

    CHECK(storage->read_4_bytes(80, 0) == 10);
}
