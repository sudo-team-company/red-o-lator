#include "RegistersTable.h"
#include <wx/propgrid/advprops.h>
#include "model/reg/RegData.h"

RegistersTable::RegistersTable(wxWindow* parent)
    : wxPropertyGrid(parent,
                     wxID_ANY,
                     wxDefaultPosition,
                     wxSize(300, 250),
                     wxPG_SPLITTER_AUTO_CENTER | wxPG_NO_INTERNAL_BORDER) {
    stateCategory = new wxPropertyCategory("State Registers");
    Append(stateCategory);

    scalarCategory = new wxPropertyCategory("Scalar Registers");
    Append(scalarCategory);

    vectorCategory = new wxPropertyCategory("Vector Registers");
    Append(vectorCategory);

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

wxPGProperty* RegistersTable::createUIntRegister(const std::string& name, uint64_t value) {
    auto reg = new wxUIntProperty(name, wxPG_LABEL, wxULongLong(value));
    SetPropertyReadOnly(reg, true);
    reg->SetAttribute(wxPG_UINT_BASE, wxPG_BASE_HEX);
    reg->SetAttribute(wxPG_UINT_PREFIX, wxPG_PREFIX_0x);
    return reg;
}

wxPGProperty* RegistersTable::createBoolRegister(const std::string& name, bool value) {
    auto reg = new wxBoolProperty(name, wxPG_LABEL, value);
    SetPropertyReadOnly(reg, true);
    return reg;
}

void RegistersTable::updateValues(const RegData& data) {
    stateCategory->DeleteChildren();
    scalarCategory->DeleteChildren();
    vectorCategory->DeleteChildren();

    SetCurrentCategory(stateCategory);
    Append(createUIntRegister("reladdr", data.RELADDR));
    Append(createUIntRegister("exec", data.EXEC));
    Append(createUIntRegister("vcc", data.VCC));
    Append(createUIntRegister("m0", data.M0));
    Append(createUIntRegister("simm16", data.SIMM16));
    Append(createUIntRegister("scc", data.SCC));

    SetCurrentCategory(scalarCategory);
    for (size_t i = 0; i < data.sFile.size(); i++) {
        auto prop = createUIntRegister("s" + std::to_string(i), data.sFile[i]);
        Append(prop);
    }

    SetCurrentCategory(vectorCategory);
    for (size_t i = 0; i < data.vFile.size(); i++) {
        auto prop = createUIntRegister("v" + std::to_string(i), data.vFile[i]);
        Append(prop);
    }
}
