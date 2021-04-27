include_directories(CL)

set(OCL_RUNTIME_SOURCES
        runtime/runtime-other.cpp runtime/runtime-platform.cpp runtime/runtime-device.cpp runtime/runtime-context.cpp runtime/runtime-command-queue.cpp runtime/runtime-memory.cpp runtime/runtime-sampler.cpp runtime/runtime-program.cpp runtime/runtime-kernel.cpp runtime/runtime-event.cpp runtime/runtime-command.cpp)

add_library(driver SHARED ${OCL_RUNTIME_SOURCES})

target_include_directories (driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(OCL_TEST_SOURCES
        test/driver-call-test.cpp)

add_executable(driver-test ${OCL_TEST_SOURCES})
target_link_libraries(driver-test PRIVATE driver)
