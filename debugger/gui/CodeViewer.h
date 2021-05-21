#ifndef RED_O_LATOR_CODEVIEWER_H
#define RED_O_LATOR_CODEVIEWER_H

#include <wx/stc/stc.h>
#include <map>
#include <set>
#include <vector>
#include "../data/Instruction.h"
#include "../data/Preferences.h"
#include "wx.h"

class MainFrame;

class CodeViewer : public wxStyledTextCtrl {
   private:
    const Preferences& preferences;

    enum Margins {
        MARGIN_LINE,
        MARGIN_MARKERS,
        MARGIN_ADDRESS,
    };

    enum Markers {
        MARKER_BREAKPOINT,
        MARKER_EXECUTION_LINE,
        MARKER_EXECUTION_CURSOR,
    };
    static constexpr int allMarkers[] = {
        MARKER_BREAKPOINT, MARKER_EXECUTION_LINE, MARKER_EXECUTION_CURSOR};

    const int MARGIN_ADDRESS_STYLE = 17;
    const int MARGIN_LABEL_STYLE = 18;
    const int NULL_MASK = 0;

    void setUpTextEditor();
    void setUpMargins();
    void setUpMarkers();

    void setExecutionMarker(int line);
    void setBreakpointMarker(int line);
    void clearBreakpointMarker(int line);

    std::map<size_t, int> addressMap;
    std::map<int, size_t> lineMap;
    std::set<size_t> breakpoints;

    const MainFrame* frame;

   public:
    explicit CodeViewer(MainFrame*, const Preferences&);

    void OnModified(wxStyledTextEvent& event);
    void OnMarginClick(wxStyledTextEvent& event);

    void adjustAddressMargin();

    void setAddress(int line, size_t value);
    void setLabel(int line, const wxString& name);

    void setInstructions(const std::vector<Instruction>& instructions);
    void setExecutionMarker(size_t address);

    void clearExecutionMarker();
};

#endif  // RED_O_LATOR_CODEVIEWER_H
