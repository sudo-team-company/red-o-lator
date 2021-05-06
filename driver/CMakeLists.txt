include_directories(CL)
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
        src/runtime/runtime-other.cpp
        src/runtime/icd.h
        src/runtime/IcdDispatchTable.cpp
        src/runtime/IcdDispatchTable.h
        src/model/GPUModel.cpp
        src/model/GPUModel.h
        src/runtime/device/DeviceConfigurationParser.cpp
        src/runtime/device/DeviceConfigurationParser.h
        src/runtime/runtime-device.cpp
        src/runtime/runtime-commons.h
        src/runtime/runtime-commons.cpp
        src/common/common.hpp)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)
add_library(driver SHARED ${OCL_RUNTIME_SOURCES})
target_include_directories(driver PUBLIC src/runtime)

add_executable(driver-exec ${OCL_RUNTIME_SOURCES} src/main.cpp)

set(OCL_TEST_SOURCES
        test/driver-call-test.cpp)

add_executable(driver-test ${OCL_TEST_SOURCES})
target_link_libraries(driver-test PRIVATE driver)
