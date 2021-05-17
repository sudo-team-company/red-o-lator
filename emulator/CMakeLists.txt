find_package(OpenCL REQUIRED)

add_library(red-o-lator-emulator
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
        )
target_link_libraries(red-o-lator-emulator PRIVATE OpenCL::OpenCL red-o-lator-common)
target_include_directories(red-o-lator-emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(util_test
        test/util/util_test.cpp
        util/util.cpp
        )
target_link_libraries(util_test PRIVATE red-o-lator-emulator)

add_executable(alu_test
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        util/util.cpp
        test/alu/alu_test.cpp
        )
target_link_libraries(alu_test PRIVATE red-o-lator-emulator)