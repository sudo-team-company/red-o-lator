set(DEBUGGER_SOURCES
        main.cpp EmulatorWindow.cpp MainFrame.cpp
        wx.h MainFrame.h EmulatorWindow.h resources/icons.rc)

add_executable(debugger ${DEBUGGER_SOURCES})

if(WIN32)
    if(NOT (MSVC OR MINGW))
        set(MSVC vc)
        set(MSVC_TOOLSET_VERSION 16)
    endif()
endif()

find_package(wxWidgets REQUIRED)

include_directories(${wxWidgets_INCLUDE_DIRS})

target_link_libraries(debugger ${wxWidgets_LIBRARIES})
target_link_libraries(debugger red-o-lator-common)