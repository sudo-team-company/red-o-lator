set(DEBUGGER_SOURCES
  main.cpp
)

add_executable(debugger ${DEBUGGER_SOURCES})
target_link_libraries(debugger red-o-lator-common)
