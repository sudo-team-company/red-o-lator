#include "Toolbar.h"
#include <wx/artprov.h>
#include "Events.h"
#include "MainFrame.h"

Toolbar::Toolbar(wxWindow* parent)
    : wxAuiToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    auto playImg = loadPNG(wxString("play"));
    auto pauseImg = loadPNG(wxString("pause"));
    auto forwardImg = loadPNG(wxString("forward"));
    auto stopImg = loadPNG(wxString("stop"));
    auto nextImg = loadPNG(wxString("next"));

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

wxImage Toolbar::loadPNG(const wxString& resourceName) {
    return wxBitmap(resourceName, wxBITMAP_TYPE_PNG_RESOURCE)
        .ConvertToImage()
        .Rescale(toolbarElementSize.GetWidth(), toolbarElementSize.GetHeight());
}
