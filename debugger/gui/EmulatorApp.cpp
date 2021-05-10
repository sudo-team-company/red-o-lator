#include "EmulatorApp.h"
#include "MainFrame.h"

bool EmulatorApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());

    auto* frame = new MainFrame("AMD Emulator", wxSize(1440, 900), preferences);

    frame->Center();
    frame->SetIcon(wxICON(icon));
    frame->Show(true);

    return true;
}