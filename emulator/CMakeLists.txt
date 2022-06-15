##################
## Main library ##
##################
include_directories(src)

add_library(red-o-lator-emulator
        SHARED
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
        src/alu/alu_vopc.cpp
        src/alu/alu_flat.cpp
        src/flow/register.cpp
        src/gpu/dispatcher.cpp
        src/gpu/compute_unit.cpp
        src/gpu/storage.cpp
        src/commons/util.cpp
        src/commons/commons.cpp
        src/Executor.cpp src/Executor.h src/commons/parser.h src/commons/parser.cpp src/flow/debug_context.cpp src/flow/debug_context.h)
target_link_libraries(red-o-lator-emulator PRIVATE red-o-lator-common)
target_include_directories(red-o-lator-emulator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

###########
## Tests ##
###########

#############
# Util test #
#############
add_executable(red-o-lator-emulator-util-test
        test/util/util_test.cpp
        src/commons/commons.cpp
        src/commons/util.cpp
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
        src/commons/util.cpp
        test/alu/sop1_test.cpp
        test/alu/sop2_test.cpp
        test/alu/sopc_test.cpp
        test/alu/sopp_test.cpp
        test/alu/vop1_test.cpp
        test/alu/vop2_test.cpp
        test/alu/vop3_test.cpp
        test/alu/vopc_test.cpp
        test/alu/runner.cpp
        src/flow/wavefront.cpp
        src/flow/register.cpp
        src/commons/commons.cpp
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
        test/execution/wf_exec_test.cpp
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
        src/alu/alu_vopc.cpp
        src/alu/alu_flat.cpp
        src/gpu/compute_unit.cpp
        src/gpu/dispatcher.cpp
        src/gpu/storage.cpp
        src/commons/util.cpp
        src/flow/wavefront.cpp
        src/flow/register.cpp
        src/commons/commons.cpp
        )

target_link_libraries(red-o-lator-emulator-wf-test PRIVATE
        red-o-lator-emulator red-o-lator-common
        )
add_test(NAME red-o-lator-emulator-wf-test
        COMMAND red-o-lator-emulator-wf-test
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-emulator-wf-test>)