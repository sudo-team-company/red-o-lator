##############
## Packages ##
##############

###################
# Common packages #
###################
find_package(PkgConfig REQUIRED)
find_package(OpenCL REQUIRED)

################
# CLRX library #
################
message(STATUS "Looking for CLRX .pc files in $ENV{PKG_CONFIG_PATH}")
pkg_check_modules(LIB_CLRX_AMD_ASM REQUIRED IMPORTED_TARGET clrxamdasm)
pkg_check_modules(LIB_CLRX_AMD_BIN REQUIRED IMPORTED_TARGET clrxamdbin)
pkg_check_modules(LIB_CLRX_UTILS REQUIRED IMPORTED_TARGET clrxutils)


##################
## Main library ##
##################
include_directories(src)

set(SOURCES
        src/runtime/CLObjectInfoParameterValue.hpp
        src/runtime/command/BufferReadCommand.cpp
        src/runtime/command/BufferWriteCommand.cpp
        src/runtime/command/Command.h
        src/runtime/device/DeviceConfigurationParser.cpp
        src/runtime/device/DeviceConfigurationParser.h
        src/runtime/device/DeviceConfigurationParser.tpp
        src/runtime/icd/CLCommandQueue.cpp
        src/runtime/icd/CLCommandQueue.h
        src/runtime/icd/CLContext.h
        src/runtime/icd/CLDeviceId.hpp
        src/runtime/icd/CLEvent.h
        src/runtime/icd/CLMem.h
        src/runtime/icd/CLPlatformId.hpp
        src/runtime/icd/CLProgram.hpp
        src/runtime/icd/CLSampler.hpp
        src/runtime/icd/IcdDispatchTable.cpp
        src/runtime/icd/IcdDispatchTable.h
        src/runtime/icd/icd.h
        src/runtime/icd/kernel/CLKernel.cpp
        src/runtime/icd/kernel/CLKernel.h
        src/runtime/icd/kernel/KernelArgument.hpp
        src/runtime/icd/kernel/KernelArgumentInfo.cpp
        src/runtime/icd/kernel/KernelArgumentInfo.h
        src/runtime/icd/kernel/KernelArgumentValue.hpp
        src/runtime/program/BinaryAsmParser.cpp
        src/runtime/program/BinaryAsmParser.h
        src/runtime/program/BinaryDisassembler.cpp
        src/runtime/program/BinaryDisassembler.h
        src/runtime/program/KernelArgumentInfoParser.cpp
        src/runtime/program/KernelArgumentInfoParser.h
        src/runtime/runtime-command-queue.cpp
        src/runtime/runtime-command.cpp
        src/runtime/runtime-commons.cpp
        src/runtime/runtime-commons.h
        src/runtime/runtime-context.cpp
        src/runtime/runtime-device.cpp
        src/runtime/runtime-event.cpp
        src/runtime/runtime-kernel.cpp
        src/runtime/runtime-memory-buffer.cpp
        src/runtime/runtime-memory-image.cpp
        src/runtime/runtime-memory.cpp
        src/runtime/runtime-platform.cpp
        src/runtime/runtime-program.cpp
        src/runtime/runtime-sampler.cpp
        src/runtime/icd/CLMem.cpp src/runtime/command/KernelExecutionCommand.cpp src/runtime/command/Command.cpp src/runtime/icd/CLEvent.cpp src/runtime/command/BufferFillCommand.cpp src/runtime/command/BufferCopyCommand.cpp)

set(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources)

add_library(red-o-lator-icd SHARED ${SOURCES})
target_link_libraries(red-o-lator-icd PUBLIC INTERFACE
        OpenCL::OpenCL
        )
target_link_libraries(red-o-lator-icd PRIVATE
        red-o-lator-common
        PkgConfig::LIB_CLRX_AMD_ASM
        )
target_include_directories(red-o-lator-icd PRIVATE src/runtime)
install(TARGETS red-o-lator-icd DESTINATION lib)


####################
# Dummy executable #
####################
add_executable(driver-disasm-test test/disasm/disasm-test.cpp)
target_link_libraries(driver-disasm-test
        red-o-lator-icd
        PkgConfig::LIB_CLRX_AMD_BIN
        )


#################
##    TESTS    ##
#################
set(TEST_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/resources)

##############
# Unit tests #
##############
set(UNIT_TEST_SOURCES
        test/unit/runner.cpp
        test/unit/runtime/runtime-platform-test.cpp
        test/unit/unit-test-common/test-commons.h
        test/unit/runtime/runtime-commons-test.cpp
        test/unit/runtime/runtime-device-test.cpp
        test/unit/unit-test-common/test-commons.cpp
        test/unit/runtime/runtime-context-test.cpp
        test/unit/runtime/runtime-command-queue-test.cpp
        test/unit/runtime/runtime-memory-test.cpp
        test/unit/runtime/runtime-memory-buffer-test.cpp
        test/unit/runtime/runtime-program-test.cpp test/unit/runtime/runtime-kernel-test.cpp)

add_executable(red-o-lator-icd-test-unit ${UNIT_TEST_SOURCES})
target_link_libraries(red-o-lator-icd-test-unit PRIVATE red-o-lator-icd red-o-lator-common)
target_include_directories(red-o-lator-icd-test-unit PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/unit/)

add_custom_command(TARGET red-o-lator-icd-test-unit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEST_RESOURCES}
        $<TARGET_FILE_DIR:red-o-lator-icd-test-unit>/test/resources
        )
add_test(NAME red-o-lator-icd-test-unit COMMAND red-o-lator-icd-test-unit
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-icd-test-unit>)

#############
# E2E tests #
#############
set(E2E_TEST_SOURCES
        test/e2e/runner.cpp
        test/e2e/runtime/runtime-e2e-test.cpp
        )

add_executable(red-o-lator-icd-test-e2e ${E2E_TEST_SOURCES})
target_link_libraries(red-o-lator-icd-test-e2e PRIVATE OpenCL::OpenCL red-o-lator-common)
add_custom_command(TARGET red-o-lator-icd-test-e2e POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${TEST_RESOURCES}
        $<TARGET_FILE_DIR:red-o-lator-icd-test-e2e>/test/resources
        )
add_test(NAME red-o-lator-icd-test-e2e COMMAND red-o-lator-icd-test-e2e
        --config $<CONFIG>
        --exe $<TARGET_FILE:red-o-lator-icd-test-e2e>
        WORKING_DIRECTORY $<TARGET_FILE_DIR:red-o-lator-icd-test-e2e>)