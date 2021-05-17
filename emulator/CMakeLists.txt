find_package(OpenCL REQUIRED)

add_library(red-o-lator-emulator
        util/util.cpp
        flow/wavefront.cpp
        gpu/dispatcher.cpp
        alu/alu_sop1.cpp
        alu/alu.cpp
        alu/alu_sop2.cpp
        alu/alu_sopp.cpp
        alu/alu_sopc.cpp
        alu/alu_sopk.cpp
        alu/alu_smem.cpp
        alu/alu_vop1.cpp
        alu/alu_vop2.cpp
        alu/alu_vop3.cpp
        alu/alu_flat.cpp
        reg/register.cpp
        instr/instruction.cpp
        instr/instr_info.cpp
        gpu/compute_unit.cpp
        gpu/storage.cpp
        main/KernelLoader.cpp
        main/KernelLoader.h
        main.cpp
        )
target_link_libraries(red-o-lator-emulator PRIVATE OpenCL::OpenCL red-o-lator-common)
target_include_directories(red-o-lator-emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(util_test
        test/util/util_test.cpp
        util/util.cpp
        )
target_link_libraries(util_test PRIVATE red-o-lator-emulator red-o-lator-common)

add_executable(alu_test
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        alu/alu_vop1.cpp
        util/util.cpp
        test/alu/alu_test.cpp
        flow/wavefront.cpp
        reg/register.cpp
        instr/instruction.cpp
        instr/instr_info.cpp
        )
target_link_libraries(alu_test PRIVATE red-o-lator-emulator red-o-lator-common)

add_executable(wf_exec_test
        test/wavefront_exec/wf_exec_test.cpp
        gpu/compute_unit.cpp
        alu/alu.cpp
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        alu/alu_sopc.cpp
        alu/alu_sopk.cpp
        alu/alu_sopp.cpp
        alu/alu_smem.cpp
        alu/alu_vop1.cpp
        alu/alu_vop2.cpp
        alu/alu_vop3.cpp
        alu/alu_flat.cpp
        gpu/storage.cpp
        util/util.cpp
        flow/wavefront.cpp
        reg/register.cpp
        instr/instruction.cpp
        instr/instr_info.cpp
        gpu/dispatcher.cpp
        ../common/common/common.hpp
        )
