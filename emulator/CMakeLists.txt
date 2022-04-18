find_package(OpenCL REQUIRED)

##################
## Main library ##
##################
add_library(red-o-lator-emulator
        src/util/util.cpp
        src/flow/wavefront.cpp
        src/alu/alu_sop1.cpp
        src/alu/alu.cpp
        src/alu/alu_sop2.cpp
        src/alu/alu_sopp.cpp
        src/alu/alu_sopc.cpp
        src/alu/alu_sopk.cpp
        src/alu/alu_smem.cpp
        src/alu/alu_vop1.cpp
        src/alu/alu_vop2.cpp
        src/alu/alu_vop3.cpp
        src/alu/alu_flat.cpp
        src/reg/register.cpp
        src/instr/instruction.cpp
        src/instr/instr_info.cpp
        src/gpu/dispatcher.cpp
        src/gpu/compute_unit.cpp
        src/gpu/storage.cpp
        src/commons.cpp
        src/main/KernelLoader.cpp
        src/main/KernelLoader.h
        )
target_link_libraries(red-o-lator-emulator PRIVATE OpenCL::OpenCL red-o-lator-common)
target_include_directories(red-o-lator-emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

###################
# Test executable #
###################
add_executable(red-o-lator-emulator-exec src/main/main.cpp src/main/KernelLoader.cpp src/main/KernelLoader.h)
target_link_libraries(red-o-lator-emulator-exec PRIVATE red-o-lator-emulator red-o-lator-common)


###########
## Tests ##
###########

#############
# Util test #
#############
add_executable(red-o-lator-emulator-util-test
        test/util/util_test.cpp
        src/util/util.cpp
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
        src/alu/alu_sop1.cpp
        src/alu/alu_sop2.cpp
        src/alu/alu_vop1.cpp
        src/util/util.cpp
        test/alu/alu_test.cpp
        src/flow/wavefront.cpp
        src/reg/register.cpp
        src/instr/instruction.cpp
        src/instr/instr_info.cpp
        src/commons.cpp
        )
target_link_libraries(red-o-lator-emulator-alu-test PRIVATE
        red-o-lator-emulator red-o-lator-common
        )
add_test(NAME red-o-lator-emulator-alu-test
        COMMAND red-o-lator-emulator-alu-test
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-emulator-alu-test>)

#############
# Wavefront execution test #
#############
add_executable(red-o-lator-emulator-wf-test
        test/wavefront_exec/wf_exec_test.cpp
        src/alu/alu.cpp
        src/alu/alu_sop1.cpp
        src/alu/alu_sop2.cpp
        src/alu/alu_sopc.cpp
        src/alu/alu_sopk.cpp
        src/alu/alu_sopp.cpp
        src/alu/alu_smem.cpp
        src/alu/alu_vop1.cpp
        src/alu/alu_vop2.cpp
        src/alu/alu_vop3.cpp
        src/alu/alu_flat.cpp
        src/gpu/compute_unit.cpp
        src/gpu/dispatcher.cpp
        src/gpu/storage.cpp
        src/util/util.cpp
        src/flow/wavefront.cpp
        src/reg/register.cpp
        src/instr/instruction.cpp
        src/instr/instr_info.cpp
        src/commons.cpp
        )

target_link_libraries(red-o-lator-emulator-wf-test PRIVATE
        red-o-lator-emulator red-o-lator-common
        )
add_test(NAME red-o-lator-emulator-wf-test
        COMMAND red-o-lator-emulator-alu-test
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-emulator-wf-test>)