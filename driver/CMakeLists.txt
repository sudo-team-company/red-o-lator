find_package(PkgConfig REQUIRED)

message(STATUS "Looking for CLRX .pc files in $ENV{PKG_CONFIG_PATH}")
pkg_check_modules(LIB_CLRX_AMD_ASM REQUIRED IMPORTED_TARGET clrxamdasm)
pkg_check_modules(LIB_CLRX_AMD_BIN REQUIRED IMPORTED_TARGET clrxamdbin)
pkg_check_modules(LIB_CLRX_UTILS REQUIRED IMPORTED_TARGET clrxutils)


include_directories(CL)
find_package(OpenCL REQUIRED)

include_directories(src)


set(OCL_RUNTIME_SOURCES
        src/runtime/runtime-platform.cpp
        src/runtime/runtime-context.cpp
        src/runtime/runtime-command-queue.cpp
        src/runtime/runtime-memory.cpp
        src/runtime/runtime-sampler.cpp
        src/runtime/runtime-program.cpp
        src/runtime/runtime-kernel.cpp
        src/runtime/runtime-event.cpp
        src/runtime/runtime-command.cpp
        src/runtime/runtime-device.cpp
        src/runtime/runtime-commons.h
        src/runtime/runtime-commons.cpp
        src/runtime/runtime-other.cpp
        src/runtime/icd/icd.h
        src/runtime/icd/IcdDispatchTable.cpp
        src/runtime/icd/IcdDispatchTable.h
        src/runtime/device/DeviceConfigurationParser.h
        src/runtime/device/DeviceConfigurationParser.cpp
        src/runtime/device/DeviceConfigurationParser.tpp
        src/runtime/runtime-memory-buffer.cpp
        src/runtime/runtime-memory-image.cpp
        src/runtime/command/Command.h
        src/runtime/icd/CLCommandQueue.cpp
        src/runtime/icd/CLCommandQueue.h
        src/runtime/command/BufferReadCommand.cpp
        src/runtime/command/BufferWriteCommand.cpp
        src/runtime/CLObjectInfoParameterValue.hpp)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

add_library(red-o-lator-icd SHARED ${OCL_RUNTIME_SOURCES})
target_link_libraries(red-o-lator-icd INTERFACE
        OpenCL::OpenCL
        )
target_link_libraries(red-o-lator-icd PRIVATE
        red-o-lator-common
        PkgConfig::LIB_CLRX_AMD_ASM
        )
target_include_directories(red-o-lator-icd PRIVATE src/runtime)

install(TARGETS red-o-lator-icd DESTINATION lib)


set(OCL_TEST_SOURCES
        test/driver-call-test.cpp
        )

add_executable(driver-test ${OCL_TEST_SOURCES})
target_link_libraries(driver-test PRIVATE red-o-lator-icd red-o-lator-common)

add_executable(driver-disasm-test test/disasm/disasm-test.cpp)
target_link_libraries(driver-disasm-test
        red-o-lator-icd
        PkgConfig::LIB_CLRX_AMD_BIN
        )
