set(COMMON_SOURCES
        common/utils/common.hpp
        common/logger/Logger.cpp
        common/logger/Logger.h
        common/test/doctest.h
        common/utils/string-utils.hpp
        common/utils/vector-utils.hpp
        common/utils/file-utlis.hpp
        common/utils/optional-utlis.hpp
        common/utils/map-utils.hpp
        common/utils/set-utlis.hpp
        common/utils/time-utils.hpp)

add_library(red-o-lator-common SHARED ${COMMON_SOURCES})
target_include_directories(red-o-lator-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
