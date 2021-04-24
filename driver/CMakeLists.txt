include_directories(src)

set(OCL_RUNTIME_SOURCES
        runtime/runtime-impl.cpp
)

add_library(driver SHARED ${OCL_RUNTIME_SOURCES})

target_include_directories (driver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
