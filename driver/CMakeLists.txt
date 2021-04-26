include_directories(CL)

set(OCL_RUNTIME_SOURCES
        runtime/runtime-impl.cpp)

add_library(driver SHARED ${OCL_RUNTIME_SOURCES})

target_include_directories (driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(OCL_TEST_SOURCES
        test/driver-call-test.cpp)

add_executable(driver-test ${OCL_TEST_SOURCES})
target_link_libraries(driver-test PRIVATE driver)
