cmake_minimum_required(VERSION 3.10)
project(red-o-lator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "FATAL: In-source builds are not allowed.
Use `cmake -H. -Bbuild` to build project or configure your IDE properly.")
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_INCLUDE_PATH})

add_compile_definitions(CL_TARGET_OPENCL_VERSION=120)

add_subdirectory(common)
add_subdirectory(driver)
add_subdirectory(emulator)
add_subdirectory(debugger)
