#include "ui/Tabs/InstallerTab.hpp"

namespace Mara::ui {

    InstallerTab::InstallerTab() {
        tablist = new brls::List();
        if(!pm::isInApplicationMode()){
            brls::ListItem* appletinstall = new brls::ListItem("main/installer/appletinstallwarning/btntitle"_i18n,
                                                               "main/installer/appletinstallwarning/btnsubtitle"_i18n);
            appletinstall->getClickEvent()->subscribe([](brls::View *view) {
                brls::Logger::debug("Instalando arrancador..");
                brls::Dialog* dialog = new brls::Dialog("main/installer/appletinstallwarning/message"_i18n);

                brls::GenericEvent::Callback noCallback = [dialog](brls::View* view) {
                    dialog->close();
                };

                brls::GenericEvent::Callback yesCallback = [dialog](brls::View* view) {
                    dialog->close();
                    brls::Application::notify("En el proximo arranque del juego lanzará automaticamente el parcheador.");
                };

                dialog->addButton("main/installer/appletinstallwarning/yes"_i18n, yesCallback);
                dialog->addButton("main/installer/appletinstallwarning/no"_i18n, noCallback);

                dialog->setCancelable(true);
                dialog->open();
            });
            tablist->addView(appletinstall);
        }
    }

    brls::List *InstallerTab::GetTab() {
        return tablist;
    }
}