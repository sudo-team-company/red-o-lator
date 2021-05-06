#include "EmulatorWindow.h"
#include "MainFrame.h"


bool EmulatorWindow::OnInit() {
    auto* frame = new MainFrame("AMD Emulator", wxSize(640, 480));
    frame->Show(true);
    return true;
}