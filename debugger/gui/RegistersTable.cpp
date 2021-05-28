#include "RegistersTable.h"
#include <wx/propgrid/advprops.h>

RegistersTable::RegistersTable(wxWindow* parent)
    : wxPropertyGrid(parent,
                     wxID_ANY,
                     wxDefaultPosition,
                     wxSize(300, 250),
                     wxPG_SPLITTER_AUTO_CENTER | wxPG_NO_INTERNAL_BORDER) {
    // TODO: get registers from emulator
    Append(new wxPropertyCategory("State Registers"));

    createUIntRegister("vcc");
    createBoolRegister("vccz");
    createBoolRegister("scc");
    createUIntRegister("exec");
    createBoolRegister("execz");

    Append(new wxPropertyCategory("Scalar Registers"));

    for (int reg = 0; reg < 104; reg++) {
        createUIntRegister("s" + std::to_string(reg));
    }

    Append(new wxPropertyCategory("Vector Registers"));

    for (int reg = 0; reg < 256; reg++) {
        createUIntRegister("v" + std::to_string(reg));
    }

    SetMarginColour("WHITE");
}

void RegistersTable::setUIntRegisterValue(std::string name, uint32_t value) {
    SetPropertyValue(wxString(name), static_cast<int>(value));
}

void RegistersTable::setULLRegisterValue(std::string name, uint64_t value) {
    SetPropertyValue(wxString(name), wxULongLong(value));
}

void RegistersTable::setBoolRegisterValue(std::string name, bool value) {
    SetPropertyValue(wxString(name), value);
}

void RegistersTable::createUIntRegister(std::string name) {
    auto reg = Append(new wxUIntProperty(name, wxPG_LABEL));
    SetPropertyReadOnly(reg, true);
    reg->SetValueFromInt(0);
    reg->SetAttribute(wxPG_UINT_BASE, wxPG_BASE_HEX);
    reg->SetAttribute(wxPG_UINT_PREFIX, wxPG_PREFIX_0x);
}

void RegistersTable::createBoolRegister(std::string name) {
    auto reg = Append(new wxBoolProperty(name, wxPG_LABEL));
    SetPropertyReadOnly(reg, true);
    reg->SetValue(false);
}
