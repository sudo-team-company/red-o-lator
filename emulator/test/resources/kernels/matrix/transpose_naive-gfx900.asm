/* Disassembling 'transpose_naive-gfx900.bin' */
.amdcl2
.gpu GFX901
.64bit
.arch_minor 0
.arch_stepping 1
.driver_version 226400
.kernel transpose_naive
    .config
        .dims xy
        .sgprsnum 17
        .vgprsnum 5
        .floatmode 0xc0
        .pgmrsrc1 0x00ac0081
        .pgmrsrc2 0x00000994
        .dx10clamp
        .ieeemode
        .useargs
        .usesetup
        .priority 0
        .arg _.global_offset_0, "size_t", long
        .arg _.global_offset_1, "size_t", long
        .arg _.global_offset_2, "size_t", long
        .arg _.printf_buffer, "size_t", void*, global, , rdonly
        .arg _.vqueue_pointer, "size_t", long
        .arg _.aqlwrap_pointer, "size_t", long
        .arg odata, "float*", float*, global, 
        .arg idata, "float*", float*, global, , rdonly
        .arg offset, "int", int
        .arg width, "int", int
        .arg height, "int", int
    .text
/*000000000000*/ s_load_dword    s0, s[4:5], 0x4
/*000000000008*/ s_load_dword    s1, s[6:7], 0x0
/*000000000010*/ s_load_dword    s2, s[6:7], 0x8
/*000000000018*/ s_load_dwordx2  s[8:9], s[6:7], 0x40
/*000000000020*/ s_load_dword    s3, s[6:7], 0x48
/*000000000028*/ s_waitcnt       lgkmcnt(0)
/*00000000002c*/ s_and_b32       s4, s0, 0xffff
/*000000000034*/ s_bfe_u32       s0, s0, 0x100010
/*00000000003c*/ s_mul_i32       s4, s4, s10
/*000000000040*/ v_mov_b32       v2, s1
/*000000000044*/ s_mul_i32       s0, s0, s11
/*000000000048*/ v_add3_u32      v0, s4, v2, v0
/*000000000050*/ v_mov_b32       v2, s2
/*000000000054*/ v_add_u32       v3, s8, v0
/*000000000058*/ v_add3_u32      v1, s0, v2, v1
/*000000000060*/ v_cmp_lt_u32    s[0:1], v1, s3
/*000000000068*/ v_cmp_gt_u32    vcc, s9, v3
/*00000000006c*/ s_and_b64       vcc, vcc, s[0:1]
/*000000000070*/ s_and_saveexec_b64 s[0:1], vcc
/*000000000074*/ s_cbranch_execz .L224_0
/*000000000078*/ s_load_dwordx4  s[12:15], s[6:7], 0x30
/*000000000080*/ v_mul_lo_u32    v2, v1, s9
/*000000000088*/ v_add_u32       v2, v2, v3
/*00000000008c*/ v_mov_b32       v3, 0
/*000000000090*/ v_lshlrev_b64   v[2:3], 2, v[2:3]
/*000000000098*/ s_waitcnt       lgkmcnt(0)
/*00000000009c*/ v_add_co_u32    v2, vcc, s14, v2
/*0000000000a0*/ v_mov_b32       v4, s15
/*0000000000a4*/ v_addc_co_u32   v3, vcc, v4, v3, vcc
/*0000000000a8*/ global_load_dword v2, v[2:3], off
/*0000000000b0*/ v_mul_lo_u32    v0, v0, s3
/*0000000000b8*/ v_add_u32       v0, v0, v1
/*0000000000bc*/ v_mov_b32       v1, 0
/*0000000000c0*/ v_lshlrev_b64   v[0:1], 2, v[0:1]
/*0000000000c8*/ v_add_co_u32    v0, vcc, s12, v0
/*0000000000cc*/ v_mov_b32       v3, s13
/*0000000000d0*/ v_addc_co_u32   v1, vcc, v3, v1, vcc
/*0000000000d4*/ s_waitcnt       vmcnt(0)
/*0000000000d8*/ global_store_dword v[0:1], v2, off
.L224_0:
/*0000000000e0*/ s_endpgm
