set(COMMON_SOURCES
        common/common.hpp
        common/Logger.cpp
        common/Logger.h
        common/test/doctest.h
        )

add_library(red-o-lator-common SHARED ${COMMON_SOURCES})
target_include_directories(red-o-lator-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})