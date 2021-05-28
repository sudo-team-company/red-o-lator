#ifndef RED_O_LATOR_COLUMNLABELS_H
#define RED_O_LATOR_COLUMNLABELS_H

#include "../wx.h"

class ColumnLabels : public wxScrolledWindow {
   public:
    explicit ColumnLabels(wxWindow* parent);

   private:
    void OnDraw(wxDC& dc) override;

    static wxString columns;
};

#endif  // RED_O_LATOR_COLUMNLABELS_H
