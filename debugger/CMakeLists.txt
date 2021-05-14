if (WIN32)
    if (NOT (MSVC OR MINGW))
        set(MSVC vc)
        set(MSVC_TOOLSET_VERSION 16)
    endif ()
endif ()

find_package(wxWidgets REQUIRED COMPONENTS core base stc scintilla aui propgrid)

include(${wxWidgets_USE_FILE})

set(DEBUGGER_SOURCES
        main.cpp gui/EmulatorApp.cpp gui/MainFrame.cpp
        gui/wx.h gui/MainFrame.h gui/EmulatorApp.h gui/CodeViewer.cpp gui/CodeViewer.h gui/Preferences.h gui/Toolbar.cpp gui/Toolbar.h gui/Events.h gui/MenuBar.cpp gui/MenuBar.h gui/ParametersTable.cpp gui/ParametersTable.h
        gui/resources/embedded/abort.c gui/resources/embedded/chip.c gui/resources/embedded/forward.c gui/resources/embedded/next.c gui/resources/embedded/pause.c gui/resources/embedded/play.c gui/resources/win.rc)

if (WIN32)
    add_executable(debugger WIN32 ${DEBUGGER_SOURCES})
else ()
    add_executable(debugger ${DEBUGGER_SOURCES})
endif ()

target_link_libraries(debugger ${wxWidgets_LIBRARIES})
target_include_directories(debugger PRIVATE ${wxWidgets_INCLUDE_DIRS})

target_link_libraries(debugger red-o-lator-common)