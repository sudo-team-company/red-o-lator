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
        )

add_executable(emulator_test
        test/util/util_test.cpp
        util/util.cpp
        )

add_executable(more_tests
        alu/alu_sop1.cpp
        alu/alu_sop2.cpp
        util/util.cpp
        test/alu/alu_test.cpp
        )

target_link_libraries(emulator driver)
