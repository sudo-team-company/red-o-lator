#ifndef RED_O_LATOR_CODEVIEWER_H
#define RED_O_LATOR_CODEVIEWER_H

#include <wx/stc/stc.h>
#include "Preferences.h"
#include "wx.h"

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
    const int NULL_MASK = 0;

    void setUpTextEditor();
    void setUpMargins();
    void setUpMarkers();

    void setAddress(int line, int value);
    void setExecutionMarker(int line);
    void clearExecutionMarker();
    void setBreakpointMarker(int line);
    void clearBreakpointMarker(int line);

   public:
    explicit CodeViewer(wxWindow*, const Preferences&);

    void OnModified(wxStyledTextEvent& event);
    void OnMarginClick(wxStyledTextEvent& event);

    void adjustAddressMargin();
};

#endif  // RED_O_LATOR_CODEVIEWER_H
