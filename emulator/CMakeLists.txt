include_directories(src)

set (EMULATOR_SOURCES
  main.cpp
)

add_executable(emulator ${EMULATOR_SOURCES})

target_link_libraries(emulator driver)
