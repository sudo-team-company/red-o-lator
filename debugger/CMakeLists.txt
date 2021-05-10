set(DEBUGGER_SOURCES
        main.cpp gui/EmulatorApp.cpp gui/MainFrame.cpp
        gui/wx.h gui/MainFrame.h gui/EmulatorApp.h resources/icons.rc gui/CodeViewer.cpp gui/CodeViewer.h gui/Preferences.h gui/Toolbar.cpp gui/Toolbar.h gui/Events.h gui/MenuBar.cpp gui/MenuBar.h gui/ParametersTable.cpp gui/ParametersTable.h)

add_executable(debugger ${DEBUGGER_SOURCES})

if (WIN32)
    if (NOT (MSVC OR MINGW))
        set(MSVC vc)
        set(MSVC_TOOLSET_VERSION 16)
    endif ()
endif ()

find_package(wxWidgets COMPONENTS core base stc scintilla aui propgrid REQUIRED)

include_directories(${wxWidgets_INCLUDE_DIRS})

target_link_libraries(debugger ${wxWidgets_LIBRARIES})
target_link_libraries(debugger red-o-lator-common)