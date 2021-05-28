#ifndef RED_O_LATOR_REGISTERSTABLE_H
#define RED_O_LATOR_REGISTERSTABLE_H

#include <wx/propgrid/propgrid.h>
#include <string>
#include "wx.h"

class RegistersTable : public wxPropertyGrid {
   public:
    explicit RegistersTable(wxWindow* parent);

    void setUIntRegisterValue(std::string name, uint32_t value);
    void setULLRegisterValue(std::string name, uint64_t value);
    void setBoolRegisterValue(std::string name, bool value);

   private:
    void createUIntRegister(std::string name);
    void createBoolRegister(std::string name);
};

#endif  // RED_O_LATOR_REGISTERSTABLE_H
