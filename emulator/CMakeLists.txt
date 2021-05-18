find_package(OpenCL REQUIRED)

##################
## Main library ##
##################
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

###################
# Test executable #
###################
add_executable(red-o-lator-emulator-exec main/main.cpp main/KernelLoader.cpp main/KernelLoader.h)
target_link_libraries(red-o-lator-emulator-exec PRIVATE red-o-lator-emulator red-o-lator-common)


###########
## Tests ##
###########

#############
# Util test #
#############
add_executable(red-o-lator-emulator-util-test
        test/util/util_test.cpp
        util/util.cpp
        )
target_link_libraries(red-o-lator-emulator-util-test PRIVATE
        red-o-lator-emulator red-o-lator-common
        )
add_test(NAME red-o-lator-emulator-util-test
        COMMAND red-o-lator-emulator-util-test
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-emulator-util-test>)

#############
# Alu test #
#############
add_executable(red-o-lator-emulator-alu-test
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        util/util.cpp
        test/alu/alu_test.cpp
        )
target_link_libraries(red-o-lator-emulator-alu-test PRIVATE
        red-o-lator-emulator red-o-lator-common
        )
add_test(NAME red-o-lator-emulator-alu-test
        COMMAND red-o-lator-emulator-alu-test
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-emulator-alu-test>)
