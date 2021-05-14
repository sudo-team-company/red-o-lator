#include "Toolbar.h"
#include <wx/artprov.h>
#include "Events.h"
#include "MainFrame.h"
#include "resources/embedded/abort.c"
#include "resources/embedded/chip.c"
#include "resources/embedded/forward.c"
#include "resources/embedded/next.c"
#include "resources/embedded/pause.c"
#include "resources/embedded/play.c"

Toolbar::Toolbar(wxWindow* parent)
    : wxAuiToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    auto playImg = loadToolbarIcon(play_png);
    auto pauseImg = loadToolbarIcon(pause_png);
    auto forwardImg = loadToolbarIcon(forward_png);
    auto stopImg = loadToolbarIcon(abort_png);
    auto nextImg = loadToolbarIcon(next_png);

    SetWindowStyle(wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);

    AddTool(OPEN, "Open",
            wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_OTHER,
                                     toolbarElementSize),
            "Open");
    AddTool(ABOUT, "Settings",
            wxArtProvider::GetBitmap(wxART_EXECUTABLE_FILE, wxART_OTHER,
                                     toolbarElementSize),
            "Settings");

    AddSeparator();
    AddStretchSpacer(1);
    AddTool(RUN, "Run", playImg, "Run");
    AddTool(PAUSE, "Pause", pauseImg, "Resume");
    AddTool(RESUME, "Resume", forwardImg, "Resume");
    AddTool(STEP, "Step", nextImg, "Step");
    AddTool(STOP, "Stop", stopImg, "Stop");
    EnableTool(PAUSE, false);
    EnableTool(RESUME, false);
    EnableTool(STEP, false);
    EnableTool(STOP, false);

    AddStretchSpacer(1);
    AddSpacer(55);
    Realize();
}

template <size_t N>
wxImage Toolbar::loadToolbarIcon(const unsigned char (&data)[N]) {
    size_t size = sizeof(data) / sizeof(data[0]);
    auto bitmap = wxBitmap::NewFromPNGData(data, size);
    auto img = bitmap.ConvertToImage();
    img.Rescale(toolbarElementSize.GetWidth(), toolbarElementSize.GetHeight());
    return img;
}
