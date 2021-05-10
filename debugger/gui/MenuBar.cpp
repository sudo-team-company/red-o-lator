#include "MenuBar.h"
#include "Events.h"

MenuBar::MenuBar(const Preferences& preferences) {
    auto* menuFile = new wxMenu;
    menuFile->Append(OPEN, "&Open...\tCtrl-O", "Load kernel sources from file");
    menuFile->AppendSeparator();
    menuFile->Append(EXIT, "&Exit", "Close program");

    auto* menuHelp = new wxMenu;
    menuHelp->Append(ABOUT);

    auto* menuPreferences = new wxMenu;
    addressMarginSwitch = menuPreferences->AppendCheckItem(
        TOGGLE_ADDRESS_MARGIN, "&Show instruction address");

    wxMenuBar::Append(menuFile, "&File");
    wxMenuBar::Append(menuPreferences, "&Preferences");
    wxMenuBar::Append(menuHelp, "&Help");

    setPreferredState(preferences);
}

void MenuBar::setPreferredState(const Preferences& preferences) {
    addressMarginSwitch->Check(preferences.showAddress);
}
