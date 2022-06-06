#include "CodeViewer.h"
#include "MainFrame.h"

CodeViewer::CodeViewer(MainFrame* parent, const Preferences& preferences)
    : preferences(preferences), wxStyledTextCtrl(parent), frame(parent) {
    StyleClearAll();

    setUpTextEditor();
    setUpMargins();
    setUpMarkers();

    SetScrollWidth(1);
    SetScrollWidthTracking(true);

    Bind(wxEVT_STC_MODIFIED, &CodeViewer::OnModified, this);
    Bind(wxEVT_STC_MARGINCLICK, &CodeViewer::OnMarginClick, this);
}

void CodeViewer::OnModified(wxStyledTextEvent& event) {
    int type = event.GetModificationType();
    if ((type & (wxSTC_MOD_INSERTTEXT | wxSTC_MOD_DELETETEXT)) == 0) return;
}

void CodeViewer::OnMarginClick(wxStyledTextEvent& event) {
    int pos = event.GetPosition();
    int line = LineFromPosition(pos);

    setBreakpointMarker(line);
}

void CodeViewer::adjustAddressMargin() {
    int width = preferences.showAddress ? 70 : 0;
    SetMarginWidth(MARGIN_ADDRESS, width);
}

void CodeViewer::setAddress(int line, uint64_t value) {
    MarginSetText(line, wxString::Format("0x%.6llX", value));
    MarginSetStyle(line, MARGIN_ADDRESS_STYLE);
}

void CodeViewer::setLabel(int line, const wxString& name) {
    MarginSetText(line, name);
    MarginSetStyle(line, MARGIN_LABEL_STYLE);
}

void CodeViewer::setBreakpointMarker(int line) {
    if (lineMap.find(line) != lineMap.end()) {
        auto address = lineMap[line];
        if (breakpoints.find(address) != breakpoints.end()) {
            breakpoints.erase(address);
            MarkerDelete(line, MARKER_BREAKPOINT);
            frame->onRemoveBreakpoint(address);
        } else {
            breakpoints.insert(address);
            MarkerAdd(line, MARKER_BREAKPOINT);
            frame->onSetBreakpoint(address);
        }
    }
}

void CodeViewer::clearBreakpointMarker(int line) {
    MarkerDelete(line, MARKER_BREAKPOINT);
}

void CodeViewer::setExecutionMarker(int line) {
    clearExecutionMarker();
    MarkerAdd(line, MARKER_EXECUTION_LINE);
    MarkerAdd(line, MARKER_EXECUTION_CURSOR);
}

void CodeViewer::setExecutionMarker(uint64_t address) {
    if (addressMap.find(address) != addressMap.end()) {
        setExecutionMarker(addressMap[address]);
    } else {
        clearExecutionMarker();
    }
}

void CodeViewer::clearExecutionMarker() {
    MarkerDeleteAll(MARKER_EXECUTION_LINE);
    MarkerDeleteAll(MARKER_EXECUTION_CURSOR);
}

void CodeViewer::setUpTextEditor() {
    SetReadOnly(true);

    SetWrapMode(wxSTC_WRAP_NONE);
    SetUseTabs(false);
    SetTabWidth(4);
    SetIndent(4);

    SetLexer(wxSTC_LEX_ASM);

    wxFont font(12, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_NORMAL);

    StyleSetFont(wxSTC_ASM_DEFAULT, font);
    StyleSetFont(wxSTC_ASM_COMMENT, font);
    StyleSetFont(wxSTC_ASM_NUMBER, font);
    StyleSetFont(wxSTC_ASM_STRING, font);
    StyleSetFont(wxSTC_ASM_OPERATOR, font);
    StyleSetFont(wxSTC_ASM_IDENTIFIER, font);
    StyleSetFont(wxSTC_ASM_CPUINSTRUCTION, font);
    StyleSetFont(wxSTC_ASM_MATHINSTRUCTION, font);
    StyleSetFont(wxSTC_ASM_REGISTER, font);
    StyleSetFont(wxSTC_ASM_DIRECTIVE, font);
    StyleSetFont(wxSTC_ASM_DIRECTIVEOPERAND, font);
    StyleSetFont(wxSTC_ASM_COMMENTBLOCK, font);
    StyleSetFont(wxSTC_ASM_CHARACTER, font);
    StyleSetFont(wxSTC_ASM_STRINGEOL, font);
    StyleSetFont(wxSTC_ASM_EXTINSTRUCTION, font);
    StyleSetFont(wxSTC_ASM_COMMENTDIRECTIVE, font);

    StyleSetForeground(wxSTC_ASM_STRING, wxColour(150, 0, 0));
    StyleSetForeground(wxSTC_ASM_COMMENTDIRECTIVE, wxColour(165, 105, 0));
    StyleSetForeground(wxSTC_ASM_IDENTIFIER, wxColour(40, 0, 60));
    StyleSetForeground(wxSTC_ASM_NUMBER, wxColour(0, 150, 0));
    StyleSetForeground(wxSTC_ASM_CHARACTER, wxColour(150, 0, 0));
    StyleSetForeground(wxSTC_ASM_OPERATOR, wxColour(0, 0, 150));
    StyleSetForeground(wxSTC_ASM_COMMENT, wxColour(150, 150, 150));
    StyleSetForeground(wxSTC_ASM_COMMENTBLOCK, wxColour(150, 150, 150));
    StyleSetForeground(wxSTC_ASM_DIRECTIVE, wxColour(0, 0, 200));
    StyleSetForeground(wxSTC_ASM_DIRECTIVEOPERAND, wxColour(0, 0, 200));

    //    SetKeyWords(wxSTC_ASM_DEFAULT, keywords);
}

void CodeViewer::setUpMargins() {
    SetMarginType(MARGIN_ADDRESS, wxSTC_MARGIN_TEXT);
    SetMarginType(MARGIN_LINE, wxSTC_MARGIN_NUMBER);
    SetMarginType(MARGIN_MARKERS, wxSTC_MARGIN_SYMBOL);

    SetMarginWidth(MARGIN_LINE, 35);
    SetMarginWidth(MARGIN_MARKERS, 20);
    adjustAddressMargin();

    wxFont font(10, wxFONTFAMILY_TELETYPE, wxFONTSTYLE_NORMAL,
                wxFONTWEIGHT_BOLD);
    StyleSetFont(MARGIN_ADDRESS_STYLE, font);
    StyleSetForeground(MARGIN_ADDRESS_STYLE, "SKY BLUE");
    StyleSetBackground(MARGIN_ADDRESS_STYLE, wxColour(240, 240, 240));
    StyleSetFont(MARGIN_LABEL_STYLE, font);
    StyleSetBackground(MARGIN_LABEL_STYLE, "PALE GREEN");

    SetMarginSensitive(MARGIN_MARKERS, true);

    int marginMarkersMask = NULL_MASK;
    for (auto marker : allMarkers) {
        marginMarkersMask |= 1 << marker;
    }

    SetMarginMask(MARGIN_LINE, NULL_MASK);
    SetMarginMask(MARGIN_ADDRESS, NULL_MASK);
    SetMarginMask(MARGIN_MARKERS, marginMarkersMask);
}

void CodeViewer::setUpMarkers() {
    auto red = wxColour(210, 50, 0);
    auto yellow = wxColour(250, 230, 00);
    auto gray = wxColour("gray");
    auto black = wxColour("black");

    MarkerDefine(MARKER_BREAKPOINT, wxSTC_MARK_CIRCLE, black, red);
    MarkerDefine(MARKER_EXECUTION_LINE, wxSTC_MARK_BACKGROUND, yellow, yellow);
    MarkerDefine(MARKER_EXECUTION_CURSOR, wxSTC_MARK_ARROW, gray, yellow);
}

void CodeViewer::setInstructions(const std::vector<InstructionView>& instructions) {
    addressMap.clear();
    lineMap.clear();

    MarkerDeleteAll(MARKER_BREAKPOINT);
    MarkerDeleteAll(MARKER_EXECUTION_LINE);
    MarkerDeleteAll(MARKER_EXECUTION_CURSOR);

    SetReadOnly(false);

    ClearAll();
    for (int line = 0; line < instructions.size(); line++) {
        auto& instr = instructions[line];
        if (instr.isLabel) {
            NewLine();
            setLabel(line, instr.text);
        } else {
            AddText(instr.text);
            NewLine();
            setAddress(line, instr.address);
            addressMap[instr.address] = line;
            lineMap[line] = instr.address;
        }
    }

    SetReadOnly(true);
}
