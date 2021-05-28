if (WIN32)
    if (NOT (MSVC OR MINGW))
        set(MSVC vc)
        set(MSVC_TOOLSET_VERSION 16)
    endif ()
endif ()

if (WIN32)
    set(WX_COMPONENTS core base stc scintilla aui propgrid)
else ()
    set(WX_COMPONENTS core base stc aui propgrid)
endif ()

find_package(wxWidgets COMPONENTS ${WX_COMPONENTS} REQUIRED)

include(${wxWidgets_USE_FILE})

set(DEBUGGER_SOURCES
        main.cpp gui/EmulatorApp.cpp gui/MainFrame.cpp
        gui/wx.h gui/MainFrame.h gui/EmulatorApp.h gui/CodeViewer.cpp gui/CodeViewer.h data/Preferences.h gui/Toolbar.cpp gui/Toolbar.h gui/Events.h gui/MenuBar.cpp gui/MenuBar.h gui/ParametersTable.cpp gui/ParametersTable.h
        gui/hex-view/MemoryView.h gui/hex-view/AddressRows.h gui/hex-view/AddressRows.cpp gui/hex-view/ColumnLabels.h gui/hex-view/ColumnLabels.cpp gui/hex-view/HexScrollWindow.h gui/hex-view/HexScrollWindow.cpp
        gui/resources/embedded/abort.c gui/resources/embedded/chip.c gui/resources/embedded/forward.c gui/resources/embedded/next.c gui/resources/embedded/pause.c gui/resources/embedded/record.c gui/resources/win.rc
        gui/hex-view/MemoryView.cpp Debugger.cpp Debugger.h gui/KernelPanel.cpp gui/KernelPanel.h data/Parameter.h data/Instruction.h gui/hex-view/hex-utils.h gui/RegistersTable.cpp gui/RegistersTable.h)

if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
    add_executable(debugger WIN32 ${DEBUGGER_SOURCES})
else ()
    add_executable(debugger ${DEBUGGER_SOURCES})
endif ()

target_link_libraries(debugger ${wxWidgets_LIBRARIES})
target_include_directories(debugger PRIVATE ${wxWidgets_INCLUDE_DIRS})

target_link_libraries(debugger red-o-lator-common)