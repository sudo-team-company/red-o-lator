#ifndef RED_O_LATOR_REGISTERSTABLE_H
#define RED_O_LATOR_REGISTERSTABLE_H

#include <wx/propgrid/propgrid.h>
#include <map>
#include <string>
#include "../data/RegDiff.h"
#include "model/reg/RegData.h"
#include "wx.h"

class RegistersTable : public wxPropertyGrid {
   public:
    explicit RegistersTable(wxWindow* parent);

    void setUIntRegisterValue(std::string name, uint32_t value);
    void setULLRegisterValue(std::string name, uint64_t value);
    void setBoolRegisterValue(std::string name, bool value);

    void updateValues(const RegData& data);

   private:
    wxPropertyCategory* stateCategory;
    wxPropertyCategory* scalarCategory;
    wxPropertyCategory* vectorCategory;

    struct Register {
        enum { INT, BOOL } type;
        wxPGProperty* pointer;
    };

    std::map<std::string, Register> registers;

    wxPGProperty* createUIntRegister(const std::string& name, uint64_t value);
    wxPGProperty* createBoolRegister(const std::string& name, bool value);
};

#endif  // RED_O_LATOR_REGISTERSTABLE_H
