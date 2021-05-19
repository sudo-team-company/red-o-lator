/* Disassembling 'a_plus_b.bin' */
.amdcl2
.gpu Iceland
.64bit
.arch_minor 0
.arch_stepping 4
.driver_version 200406
.compile_options " -D__AMD__=1 -D__Ellesmere__=1 -D__Ellesmere=1 -DFP_FAST_FMA=1 -D__IMAGE_SUPPORT__=1 -cl-denorms-are-zero -m64 -Dcl_khr_fp64=1 -Dcl_amd_fp64=1 -Dcl_khr_global_int32_base_atomics=1 -Dcl_khr_global_int32_extended_atomics=1 -Dcl_khr_local_int32_base_atomics=1 -Dcl_khr_local_int32_extended_atomics=1 -Dcl_khr_int64_base_atomics=1 -Dcl_khr_int64_extended_atomics=1 -Dcl_khr_3d_image_writes=1 -Dcl_khr_byte_addressable_store=1 -Dcl_khr_fp16=1 -Dcl_khr_gl_sharing=1 -Dcl_khr_gl_depth_images=1 -Dcl_amd_device_attribute_query=1 -Dcl_amd_vec3=1 -Dcl_amd_printf=1 -Dcl_amd_media_ops=1 -Dcl_amd_media_ops2=1 -Dcl_amd_popcnt=1 -Dcl_khr_d3d10_sharing=1 -Dcl_khr_d3d11_sharing=1 -Dcl_khr_dx9_media_sharing=1 -Dcl_khr_image2d_from_buffer=1 -Dcl_khr_spir=1 -Dcl_khr_subgroups=1 -Dcl_khr_gl_event=1 -Dcl_khr_depth_images=1 -Dcl_khr_mipmap_image=1 -Dcl_khr_mipmap_image_writes=1 -Dcl_amd_liquid_flash=1 -Dcl_amd_planar_yuv=1 "
.acl_version "AMD-COMP-LIB-v0.8 (0.0.SC_BUILD_NUMBER)"
.kernel a_plus_b
    .config
        .dims , x
        .sgprsnum 12
        .vgprsnum 3
        .floatmode 0xc0
        .pgmrsrc1 0x00ac0040
        .pgmrsrc2 0x0000000c
        .dx10clamp
        .ieeemode
        .useargs
        .priority 0
        .arg _.global_offset_0, "size_t", long
        .arg _.global_offset_1, "size_t", long
        .arg _.global_offset_2, "size_t", long
        .arg _.printf_buffer, "size_t", void*, global, , rdonly
        .arg _.vqueue_pointer, "size_t", long
        .arg _.aqlwrap_pointer, "size_t", long
        .arg a, "int*", int*, global, const, rdonly
        .arg b, "int*", int*, global, const, rdonly
        .arg c, "int*", int*, global, 
    .text
/*000000000000*/ s_load_dwordx4  s[0:3], s[4:5], 0x30
/*000000000008*/ s_waitcnt       lgkmcnt(0)
/*00000000000c*/ s_load_dword    s0, s[0:1], 0x0
/*000000000014*/ s_load_dword    s1, s[2:3], 0x0
/*00000000001c*/ s_load_dwordx2  s[2:3], s[4:5], 0x40
/*000000000024*/ s_waitcnt       lgkmcnt(0)
/*000000000028*/ s_add_u32       s0, s1, s0
/*00000000002c*/ v_mov_b32       v0, s0
/*000000000030*/ v_mov_b32       v1, s2
/*000000000034*/ v_mov_b32       v2, s3
/*000000000038*/ flat_store_dword v[1:2], v0
/*000000000040*/ s_endpgm
