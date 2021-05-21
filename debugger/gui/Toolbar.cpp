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

    choiceModel =
        new wxChoice(this, SELECT_MODEL, wxDefaultPosition, wxSize(200, 24));
    choiceModel->SetLabel("Select GPU Model");

    AddControl(choiceModel);

    AddStretchSpacer(1);

    AddTool(ATTACH, "Attach", recordImg, "Attach");
    AddTool(PAUSE, "Pause", pauseImg, "Pause");
    AddTool(RESUME, "Resume", forwardImg, "Resume");
    AddTool(STEP, "Step", nextImg, "Step");
    AddTool(STOP, "Stop", stopImg, "Stop");

    AddStretchSpacer(1);
    AddSpacer(200);

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
