set(COMMON_SOURCES
        common/common.hpp common/Logger.cpp common/Logger.h
        )

add_library(red-o-lator-common ${COMMON_SOURCES})
target_include_directories(red-o-lator-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})