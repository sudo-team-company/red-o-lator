find_package(OpenCL REQUIRED)

add_executable(emulator
        util/util.cpp
        flow/wavefront.cpp
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        alu/alu_sopp.cpp
        alu/alu_sopc.cpp
        alu/alu_sopk.cpp
        reg/register.cpp
        instr/instruction.cpp
        instr/instr_info.cpp
        instr/temp.cpp
        cu/scalar_unit.cpp
        cu/compute_unit.cpp
        cu/simd_unit.cpp
        main/KernelLoader.cpp
        main/KernelLoader.h
        main/main.cpp
        )

target_link_libraries(emulator PRIVATE OpenCL::OpenCL red-o-lator-common)


add_executable(util_test
        test/util/util_test.cpp
        util/util.cpp
        )
target_link_libraries(util_test PRIVATE red-o-lator-common)


add_executable(alu_test
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        util/util.cpp
        test/alu/alu_test.cpp
        )
target_link_libraries(alu_test PRIVATE red-o-lator-common)
