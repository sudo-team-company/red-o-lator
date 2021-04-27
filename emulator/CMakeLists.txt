set (EMULATOR_SOURCES
  main.cpp
)

add_executable(emulator ${EMULATOR_SOURCES}
        instr_set/instruction.cpp
        instr_set/register.cpp
        instr_set/register_type.h)

target_link_libraries(emulator driver)
