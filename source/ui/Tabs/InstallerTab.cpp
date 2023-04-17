#include "ui/Tabs/InstallerTab.hpp"
#include "ns/ns.hpp"

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
                    char path[255];
                    for (auto &title : Mara::ns::getAllTitles())
                    {
                        if(title.second->GetTitleID() == GAME_PID_USA || title.second->GetTitleID() == GAME_PID_EUR) {
                            sprintf(path, "sdmc:/atmosphere/contents/%016lX/", title.second->GetTitleID());
                            break;
                        }
                    }

                    Result rc = Mara::fs::createdir("sdmc:/atmosphere/contents/");
                    if(R_SUCCEEDED(rc)){
                        Mara::fs::createdir(path);
                    }

                    std::string nspgame = "exefs.nsp";
                    std::string nsppath = path + nspgame;
                    if(Mara::fs::copy_file(HBL_GAME, nsppath.c_str())) {
                        //brls::Application::notify("En el proximo arranque del juego lanzará automaticamente el parcheador.");
                        auto* dialog_done = new brls::Dialog("main/installer/appletinstalldone/message"_i18n);

                        brls::GenericEvent::Callback yesCallback_done = [dialog_done](brls::View* view){
                            for (auto &title : Mara::ns::getAllTitles())
                            {
                                if(title.second->GetTitleID() == GAME_PID_USA || title.second->GetTitleID() == GAME_PID_EUR) {
                                    title.second->Launch();
                                    break;
                                }
                            }
                        };

                        brls::GenericEvent::Callback noCallback_done = [dialog_done](brls::View* view) {
                            dialog_done->close();
                        };

                        dialog_done->addButton("main/installer/appletinstalldone/yes"_i18n, yesCallback_done);
                        dialog_done->addButton("main/installer/appletinstalldone/no"_i18n, noCallback_done);
                    }

                    dialog->close();
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