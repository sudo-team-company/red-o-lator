#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "common/test/doctest.h"

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
    const std::vector<std::string>& config = {
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
        ".arg c, \"int*\", int*, global"};
    KernelConfig kernelConfig =
        KernelConfig(1, nullptr, globalWorkSize, globalWorkSize, config);
    kernelConfig.kernArgAddr = kernArgAddr;
    const std::vector<std::string>& instructions = {
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
        "/*000000000040*/ s_endpgm"};
    KernelCode kernelCode = KernelCode(instructions);

    auto storage = Storage::get_instance();
    storage->init(
        128);  // 0 - 47 bytes contain 64-bits words: global_offset_0, global_offset_1,
               // global_offset_2, printf_buffer,vqueue_pointer, aqlwrap_pointer
    storage->write_data(48, 0, uint64_t(72));   // write address of a
    storage->write_data(48, 8, uint64_t(76));   // write address of b
    storage->write_data(48, 16, uint64_t(80));  // write address of c
    storage->write_data(72, 0, uint32_t(3));    // a = 3
    storage->write_data(76, 0, uint32_t(10));   // b = 10

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
    const std::vector<std::string>& config = {
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
        ".arg x, \"int\", int"};
    KernelConfig kernelConfig =
        KernelConfig(1, nullptr, globalWorkSize, localWorkSize, config);
    kernelConfig.kernArgAddr = kernArgAddr;
    const std::vector<std::string>& instructions = {
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
        "/*000000000054*/ s_endpgm"};
    KernelCode kernelCode = KernelCode(instructions);

    int x = 50;
    uint64_t dataAddress = 60;
    auto storage = Storage::get_instance();
    storage->init(60 + (1024 * 4));                     //[0:60] - args, [60:1084] - data
    storage->write_data(48, 0, uint64_t(dataAddress));  // write address of data
    storage->write_data(56, 0, uint32_t(x));            // write value of x

    Dispatcher dispatcher = Dispatcher(&kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }

    for (size_t i = 0; i < 1024; i += 4) {
        CHECK(storage->read_4_bytes(dataAddress, i) == 100);
    }
}

/**
 * __kernel __attribute__((reqd_work_group_size(4, 16, 2)))
 * void add_get_global_offset_x(int x, __global int *data) {
 *	uint id0 = get_global_id(0);
 *	uint id1 = get_global_id(1);
 *	uint id2 = get_global_id(2);
 *	uint g0 = get_global_offset(0);
 *	uint g1 = get_global_offset(1);
 *	uint g2 = get_global_offset(2);
 *	data[id0] = g0 + x;
 *	data[id1] = g1 + x;
 *	data[id2] = g2 + x;
 * }
 */
TEST_CASE("data[global_id(i)] = global_offset(i) + x") {
    // todo test offset
    auto kernArgAddr = 0;
    size_t globalWorkSize[3] = {6 * 5, 16 * 5, 2 * 5};
    size_t dataSize =
        std::max(globalWorkSize[0], std::max(globalWorkSize[1], globalWorkSize[2]));
    size_t localWorkSize[3] = {6, 15, 2};
    const std::vector<std::string>& config = {
        ".dims xyz",
        ".cws 4, 16, 2",
        ".sgprsnum 17",
        ".vgprsnum 14",
        ".floatmode 0xc0",
        ".pgmrsrc1 0x00ac0083",
        ".pgmrsrc2 0x0000138c",
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
        ".arg x, \"int\", int",
        ".arg data, \"int*\", int*, global,"};
    KernelConfig kernelConfig =
        KernelConfig(3, nullptr, globalWorkSize, localWorkSize, config);
    kernelConfig.kernArgAddr = kernArgAddr;
    const std::vector<std::string>& instructions = {
        "/*000000000000*/ s_load_dwordx4  s[0:3], s[4:5], 0x0",
        "/*000000000008*/ s_load_dwordx2  s[10:11], s[4:5], 0x10",
        "/*000000000010*/ s_waitcnt       lgkmcnt(0)",
        "/*000000000014*/ s_load_dword    s1, s[4:5], 0x30",
        "/*00000000001c*/ s_load_dwordx2  s[4:5], s[4:5], 0x38",
        "/*000000000024*/ s_lshl_b32      s3, s6, 2",
        "/*000000000028*/ s_lshl_b32      s6, s7, 4",
        "/*00000000002c*/ s_add_u32       s3, s3, s0",
        "/*000000000030*/ s_lshl_b32      s7, s8, 1",
        "/*000000000034*/ v_add_u32       v3, s3, v0",
        "/*000000000038*/ s_add_u32       s3, s6, s2",
        "/*00000000003c*/ v_add_u32       v6, s3, v1",
        "/*000000000040*/ s_add_u32       s3, s7, s10",
        "/*000000000044*/ s_waitcnt       lgkmcnt(0)",
        "/*000000000048*/ s_add_u32       s0, s0, s1",
        "/*00000000004c*/ v_mov_b32       v4, 0",
        "/*000000000050*/ v_lshlrev_b64   v[3:4], 2, v[3:4]",
        "/*000000000058*/ v_add_u32       v8, s3, v2",
        "/*00000000005c*/ v_add_u32       v10, s4, v3",
        "/*000000000060*/ v_mov_b32       v3, s5",
        "/*000000000064*/ v_addc_u32      v11, vcc, v3, v4, vcc",
        "/*000000000068*/ v_mov_b32       v5, s0",
        "/*00000000006c*/ s_add_u32       s0, s2, s1",
        "/*000000000070*/ v_mov_b32       v7, 0",
        "/*000000000074*/ v_lshlrev_b64   v[6:7], 2, v[6:7]",
        "/*00000000007c*/ v_add_u32       v1, s4, v6",
        "/*000000000080*/ v_addc_u32      v2, vcc, v3, v7, vcc",
        "/*000000000084*/ v_mov_b32       v7, s0",
        "/*000000000088*/ s_add_u32       s0, s10, s1",
        "/*00000000008c*/ v_mov_b32       v9, 0",
        "/*000000000090*/ v_lshlrev_b64   v[8:9], 2, v[8:9]",
        "/*000000000098*/ v_add_u32       v12, s4, v8",
        "/*00000000009c*/ v_addc_u32      v13, vcc, v3, v9, vcc",
        "/*0000000000a0*/ v_mov_b32       v8, s0",
        "/*0000000000a4*/ flat_store_dword v[10:11], v5",
        "/*0000000000ac*/ flat_store_dword v[1:2], v7",
        "/*0000000000b4*/ flat_store_dword v[12:13], v8",
        "/*0000000000bc*/ s_endpgm"};
    KernelCode kernelCode = KernelCode(instructions);

    uint64_t dataAddress = 64;
    auto storage = Storage::get_instance();
    auto memsize = dataSize * 4;
    storage->init(64 + memsize);
    storage->write_data(48, 0, uint32_t(100));          // write value of x
    storage->write_data(56, 0, uint64_t(dataAddress));  // write address of data

    Dispatcher dispatcher = Dispatcher(&kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }

    for (size_t i = 0; i < memsize; i += 4) {
        CHECK(storage->read_4_bytes(dataAddress, i) == 100);
    }
}

/**
* __kernel void weighted_sum_kernel(int n, __global float *a, __global float *b, __global
float *s, __global float *c)
{
        int i = get_global_id(2) * get_global_size(0) * get_global_size(1) +
                get_global_id(1) * get_global_size(0) + get_global_id(0);
        float m = 0.0;
        if(b != 0)
                m = b[i];
        if (i < n) {
                c[i] = s[i] * a[i] + (1 - s[i])*m;
        }
}
*/
TEST_CASE("weighted_sum_kernel") {
    // todo test offset
    auto kernArgAddr = 0;
    size_t globalWorkSize[3] = {6 * 5, 16 * 5, 2 * 5};
    size_t dataSize =
        std::max(globalWorkSize[0], std::max(globalWorkSize[1], globalWorkSize[2]));
    size_t localWorkSize[3] = {6, 15, 2};
    const std::vector<std::string>& config = {
        ".dims xyz",
        ".sgprsnum 21",
        ".vgprsnum 6",
        ".floatmode 0xc0",
        ".pgmrsrc1 0x00ac0081",
        ".pgmrsrc2 0x00001390",
        ".dx10clamp",
        ".ieeemode",
        ".useargs",
        ".usesetup",
        ".priority 0",
        ".arg _.global_offset_0, \"size_t\", long",
        ".arg _.global_offset_1, \"size_t\", long",
        ".arg _.global_offset_2, \"size_t\", long",
        ".arg _.printf_buffer, \"size_t\", void*, global, , rdonly",
        ".arg _.vqueue_pointer, \"size_t\", long",
        ".arg _.aqlwrap_pointer, \"size_t\", long",
        ".arg n, \"int\", int",
        ".arg a, \"float*\", float*, global, , rdonly",
        ".arg b, \"float*\", float*, global, , rdonly",
        ".arg s, \"float*\", float*, global, , rdonly",
        ".arg c, \"float*\", float*, global"};
    KernelConfig kernelConfig =
        KernelConfig(3, nullptr, globalWorkSize, localWorkSize, config);
    kernelConfig.kernArgAddr = kernArgAddr;
    const std::vector<std::string>& instructions = {
        "/*000000000000*/ s_load_dwordx4  s[0:3], s[4:5], 0x4",
        "/*000000000008*/ s_load_dwordx2  s[4:5], s[6:7], 0x10",
        "/*000000000010*/ s_load_dwordx4  s[12:15], s[6:7], 0x0",
        "/*000000000018*/ s_waitcnt       lgkmcnt(0)",
        "/*00000000001c*/ s_and_b32       s5, s0, 0xffff",
        "/*000000000024*/ s_bfe_u32       s0, s0, 0x100010",
        "/*00000000002c*/ s_and_b32       s1, s1, 0xffff",
        "/*000000000034*/ s_mul_i32       s5, s5, s8",
        "/*000000000038*/ s_mul_i32       s0, s0, s9",
        "/*00000000003c*/ v_add_u32       v1, s0, v1",
        "/*000000000040*/ s_mul_i32       s0, s1, s10",
        "/*000000000044*/ s_add_u32       s1, s5, s12",
        "/*000000000048*/ v_add_u32       v0, s1, v0",
        "/*00000000004c*/ s_add_u32       s0, s0, s4",
        "/*000000000050*/ v_add_u32       v2, s0, v2",
        "/*000000000054*/ v_mul_lo_u32    v2, s3, v2",
        "/*00000000005c*/ v_add_u32       v2, s14, v2",
        "/*000000000060*/ v_add_u32       v1, v2, v1",
        "/*000000000064*/ v_mul_lo_u32    v1, v1, s2",
        "/*00000000006c*/ v_add_u32       v2,  v1, v0",
        "/*000000000070*/ s_load_dwordx2  s[0:1], s[6:7], 0x40",
        "/*000000000078*/ s_waitcnt       lgkmcnt(0)",
        "/*00000000007c*/ s_cmp_eq_u64    s[0:1], 0",
        "/*000000000080*/ s_cbranch_scc1  .L168_0",
        "/*000000000084*/ v_ashrrev_i32   v3, 31, v2",
        "/*000000000088*/ v_lshlrev_b64   v[0:1], 2, v[2:3]",
        "/*000000000090*/ v_add_u32       v0, s0, v0",
        "/*000000000094*/ v_mov_b32       v3, s1",
        "/*000000000098*/ v_addc_u32      v1, vcc, v3, v1, vcc",
        "/*00000000009c*/ flat_load_dword v4, v[0:1]",
        "/*0000000000a4*/ s_branch        .L172_0",
        ".L168_0:",
        "/*0000000000a8*/ v_mov_b32       v4, 0",
        ".L172_0:",
        "/*0000000000ac*/ s_load_dword    s0, s[6:7], 0x30",
        "/*0000000000b4*/ s_waitcnt       lgkmcnt(0)",
        "/*0000000000b8*/ v_cmp_gt_i32    vcc, s0, v2",
        "/*0000000000bc*/ s_and_saveexec_b64 s[0:1], vcc",
        "/*0000000000c0*/ s_cbranch_execz .L308_0",
        "/*0000000000c4*/ s_load_dwordx2  s[2:3], s[6:7], 0x38",
        "/*0000000000cc*/ s_load_dwordx4  s[4:7], s[6:7], 0x48",
        "/*0000000000d4*/ v_ashrrev_i32   v1, 31, v2",
        "/*0000000000d8*/ v_mov_b32       v0, v2",
        "/*0000000000dc*/ v_lshlrev_b64   v[0:1], 2, v[0:1]",
        "/*0000000000e4*/ s_waitcnt       lgkmcnt(0)",
        "/*0000000000e8*/ v_add_u32       v2, s2, v0",
        "/*0000000000ec*/ v_mov_b32       v3, s3",
        "/*0000000000f0*/ v_addc_u32      v3, vcc, v3, v1, vcc",
        "/*0000000000f4*/ flat_load_dword v5, v[2:3]",
        "/*0000000000fc*/ v_add_u32       v2, s4, v0",
        "/*000000000100*/ v_mov_b32       v3, s5",
        "/*000000000104*/ v_addc_u32      v3, vcc, v3, v1, vcc",
        "/*000000000108*/ flat_load_dword v2, v[2:3]",
        "/*000000000110*/ v_add_u32       v0, s6, v0",
        "/*000000000114*/ v_mov_b32       v3, s7",
        "/*000000000118*/ v_addc_u32      v1, vcc, v3, v1, vcc",
        "/*00000000011c*/ s_waitcnt       vmcnt(0) & lgkmcnt(0)",
        "/*000000000120*/ v_sub_f32       v3, 1.0, v2",
        "/*000000000124*/ v_mul_f32       v3, v4, v3",
        "/*000000000128*/ v_mac_f32       v3, v2, v5",
        "/*00000000012c*/ flat_store_dword v[0:1], v3",
        ".L308_0:",
        "/*000000000134*/ s_endpgm"};
    KernelCode kernelCode = KernelCode(instructions);

    uint64_t dataAddress = 64;
    auto storage = Storage::get_instance();
    auto memsize = dataSize * 4;
    storage->init(64 + memsize);
    storage->write_data(48, 0, uint32_t(100));          // write value of x
    storage->write_data(56, 0, uint64_t(dataAddress));  // write address of data

    Dispatcher dispatcher = Dispatcher(&kernelConfig, &kernelCode);
    while (dispatcher.has_next_wg()) {
        std::unique_ptr<WorkGroup> workGroup(dispatcher.next_wg());
        ComputeUnit::run_work_group(workGroup.get());
    }
}