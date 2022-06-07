#include "Toolbar.h"
#include "Events.h"
#include "MainFrame.h"
#include "resources/embedded/abort.c"
#include "resources/embedded/chip.c"
#include "resources/embedded/forward.c"
#include "resources/embedded/next.c"
#include "resources/embedded/pause.c"
#include "resources/embedded/record.c"

Toolbar::Toolbar(wxWindow* parent)
    : wxAuiToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize) {
    auto recordImg = loadToolbarIcon(record_png);
    auto pauseImg = loadToolbarIcon(pause_png);
    auto forwardImg = loadToolbarIcon(forward_png);
    auto stopImg = loadToolbarIcon(abort_png);
    auto nextImg = loadToolbarIcon(next_png);

    SetWindowStyle(wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_OVERFLOW);

    wxStaticText * wfText = new wxStaticText(this, wxID_ANY, wxT("Context:"));
    AddControl(wfText);
    AddSpacer(5);

    choiceWF =
        new wxChoice(this, SELECT_WAVEFRONT, wxDefaultPosition, wxSize(100, 24));

    AddControl(choiceWF);

    AddSpacer(10);

    wxStaticText * wiText = new wxStaticText(this, wxID_ANY, wxT("Work Item"));
    AddControl(wiText);
    AddSpacer(5);

    wxStaticText * wiXText = new wxStaticText(this, wxID_ANY, wxT("X:"));
    AddControl(wiXText);
    AddSpacer(5);
    choiceWIX =
        new wxChoice(this, SELECT_WORK_ITEM_X, wxDefaultPosition, wxSize(42, 24));
    AddControl(choiceWIX);

    AddSpacer(5);

    wxStaticText * wiYText = new wxStaticText(this, wxID_ANY, wxT("Y:"));
    AddControl(wiYText);
    AddSpacer(5);
    choiceWIY =
        new wxChoice(this, SELECT_WORK_ITEM_Y, wxDefaultPosition, wxSize(42, 24));
    AddControl(choiceWIY);

    AddSpacer(5);
    wxStaticText * wiZText = new wxStaticText(this, wxID_ANY, wxT("Z:"));
    AddControl(wiZText);
    AddSpacer(5);
    choiceWIZ =
        new wxChoice(this, SELECT_WORK_ITEM_Z, wxDefaultPosition, wxSize(42, 24));
    AddControl(choiceWIZ);

    AddSpacer(5);

    AddStretchSpacer(1);

    AddTool(ATTACH, "Attach", recordImg, "Attach");
    AddTool(RESUME, "Resume", forwardImg, "Resume");
    AddTool(STEP, "Step", nextImg, "Step");
    AddTool(STOP, "Stop", stopImg, "Stop");

    AddStretchSpacer(1);
    AddSpacer(450);

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
