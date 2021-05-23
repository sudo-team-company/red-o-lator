#include "Debugger.h"
#include <iostream>
#include "gui/EmulatorApp.h"

Debugger::Debugger(EmulatorApp* app) {
    this->app = app;

    /* mock state */

    app->setKernelList({"exampleKernel1", "exampleKernel2", "exampleKernel3"},
                       2);

    app->setGlobalParameters({Parameter{"amdcl2"}, Parameter{"gpu", "Iceland"},
                              Parameter{"64bit", "0"}});
    app->setKernelParameters(
        {Parameter{"dims", "xy"}, Parameter{"sgprsnum", "16"}});

    app->setInstructions({
        Instruction{0x00, "s_load_dwordx4  s[0:3], s[4:5], 0x0"},
        Instruction{0x08, "s_waitcnt       lgkmcnt(0)"},
        Instruction{0x0c, "s_lshl_b32      s1, s6, 4"},
        Instruction{0x10, "v_add_u32       v0, vcc, s1, v0"},
        Instruction{0x14, "s_lshl_b32      s1, s7, 4"},
        Instruction{0x18, "s_add_u32       s1, s1, s2"},
        Instruction{0x1c, "v_add_u32       v0, vcc, s0, v0"},
        Instruction{0x20, "v_add_u32       v2, vcc, s1, v1"},
        Instruction{0x24, "v_cmp_eq_i32    vcc, 1, v0"},
        Instruction{0x28, "s_load_dword    s0, s[4:5], 0x30"},
        Instruction{0x30, "s_load_dword    s1, s[4:5], 0x40"},
        Instruction{0x38, "s_load_dwordx2  s[2:3], s[4:5], 0x38"},
        Instruction{0x40, "s_and_saveexec_b64 s[4:5], vcc"},
        Instruction{0x44, "v_mov_b32       v1, 0"},
        Instruction{0x48, "s_cbranch_execz .L112_0"},
        Instruction{0x4c, "v_lshlrev_b64   v[0:1], 2, v[0:1]"},
        Instruction{0x54, "s_waitcnt       lgkmcnt(0)"},
        Instruction{0x58, "v_add_u32       v0, vcc, s2, v0"},
        Instruction{0x5c, "v_mov_b32       v3, s3"},
        Instruction{0x60, "v_addc_u32      v1, vcc, v3, v1, vcc"},
        Instruction{0x64, "v_mul_lo_u32    v3, v2, s0"},
        Instruction{0x6c, "v_subrev_u32    v4, vcc, s1, v3"},
        Instruction{0, ".L112_0:", true},
        Instruction{0x70, "s_andn2_b64     exec, s[4:5], exec"},
        Instruction{0x74, "s_cbranch_execz .L156_0"},
        Instruction{0x78, "s_waitcnt       lgkmcnt(0)"},
        Instruction{0x7c, "s_mul_i32       s1, s1, s0"},
        Instruction{0x80, "v_mov_b32       v1, 0"},
        Instruction{0x84, "v_lshlrev_b64   v[0:1], 2, v[0:1]"},
        Instruction{0x8c, "v_add_u32       v0, vcc, s2, v0"},
        Instruction{0x90, "v_mov_b32       v3, s3"},
        Instruction{0x94, "v_addc_u32      v1, vcc, v3, v1, vcc"},
        Instruction{0x98, "v_mov_b32       v4, s1"},
        Instruction{0, ".L156_0:", true},
        Instruction{0x9c, "s_mov_b64       exec, s[4:5]"},
        Instruction{0xa0, "v_mov_b32       v3, 0"},
        Instruction{0xa4, "v_lshlrev_b64   v[2:3], 2, v[2:3]"},
        Instruction{0xac, "s_waitcnt       lgkmcnt(0)"},
        Instruction{0xb0, "v_add_u32       v2, vcc, s2, v2"},
        Instruction{0xb4, "v_mov_b32       v5, s3"},
        Instruction{0xb8, "v_addc_u32      v3, vcc, v5, v3, vcc"},
        Instruction{0xbc, "v_mov_b32       v5, s0"},
        Instruction{0xc0, "flat_store_dword v[0:1], v4"},
        Instruction{0xc8, "flat_store_dword v[2:3], v5"},
        Instruction{0xd0, "s_endpgm"},
    });

    app->startExecution();
    app->pauseExecution(0x70, 1);


    app->setModelChoice(
        {"Radeon RX Vega 56 (Vega10 XL)", "Radeon RX 590 (Polaris30 XT)",
         "Radeon 530", "Radeon RX 480 (Ellesmere XT)", "Radeon R9 295X2 (Vesuvius)"},
        2);
}

Debugger::~Debugger() = default;

void Debugger::onSetBreakpoint(size_t address) {
    std::cout << "set breakpoint at " << std::hex << address << std::endl;
}

void Debugger::onRemoveBreakpoint(size_t address) {
    std::cout << "remove breakpoint at " << std::hex << address << std::endl;
}

void Debugger::onSelectModel(size_t index) {
    std::cout << "select gpu " << index << std::endl;
}

void Debugger::onSelectKernel(size_t index) {
    std::cout << "select kernel " << index << std::endl;
}

void Debugger::onRequestMemory(size_t index) {
    std::cout << "request memory at " << std::hex << index << std::endl;

    auto x =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Praesent sit "
        "amet est neque. Ut eget placerat urna. Nulla elementum vehicula ante "
        "eu porttitor. Suspendisse in dapibus ex, id rutrum nisl. Proin "
        "lobortis blandit libero ullamcorper convallis. Fusce hendrerit velit "
        "sit amet quam auctor vulputate. Nullam vulputate malesuada orci eget "
        "convallis.";

    app->setMemoryView(x, strlen(x), index);
}

void Debugger::onAttach() {
    std::cout << "attach to emulator" << std::endl;
    app->startExecution();
}

void Debugger::onPause() {
    std::cout << "pause" << std::endl;
    app->pauseExecution(0x80, 3);
}

void Debugger::onResume() {
    std::cout << "resume" << std::endl;
    app->startExecution();
}

void Debugger::onStep() {
    std::cout << "step" << std::endl;
    app->pauseExecution(0x84, 0);
}

void Debugger::onStop() {
    std::cout << "stop" << std::endl;
    app->stopExecution();
}
