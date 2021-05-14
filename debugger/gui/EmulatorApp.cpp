#include "EmulatorApp.h"
#include "MainFrame.h"
#include "resources/embedded/chip.c"

bool EmulatorApp::OnInit() {
    wxImage::AddHandler(new wxPNGHandler());
    auto bitmap = wxBITMAP_PNG_FROM_DATA(chip);
    wxIcon icon;
    icon.CopyFromBitmap(bitmap);

    auto* frame = new MainFrame("AMD Emulator", wxSize(1440, 900), preferences);

    frame->Center();
    frame->SetIcon(icon);
    frame->Show(true);

    return true;
}