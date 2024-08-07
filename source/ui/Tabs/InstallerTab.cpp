#include "ui/Tabs/InstallerTab.hpp"
#include "ns/ns.hpp"

namespace Mara::ui {

    InstallerTab::InstallerTab() {
        tablist = new brls::List();
        if(!pm::isInApplicationMode()){

            char path[255];
            if(patchData->game_found){
                sprintf(path, "sdmc:/atmosphere/contents/%016lX/", patchData->program->GetTitleID());
            }

            brls::ListItem* appletinstall = new brls::ListItem("main/installer/appletinstallwarning/btntitle"_i18n,
                                                               "main/installer/appletinstallwarning/btnsubtitle"_i18n);
            appletinstall->getClickEvent()->subscribe([path](brls::View *view) {
                brls::Logger::debug("Instalando arrancador..");
                brls::Dialog* dialog = new brls::Dialog("main/installer/appletinstallwarning/message"_i18n);

                brls::GenericEvent::Callback noCallback = [dialog](brls::View* view) {
                    dialog->close();
                };

                brls::GenericEvent::Callback yesCallback = [dialog, path](brls::View* view) {
                    Result rc = Mara::fs::createdir("sdmc:/atmosphere/contents/");
                    if(R_SUCCEEDED(rc)){
                        Mara::fs::createdir(path);
                    }

                    char path2[255];
                    sprintf(path2, "%s:/atmosphere/contents/%016lX/romfs", SDCARD_MOUNT_NAME, patchData->program->GetTitleID());
                    if(Mara::fs::checkdirexist(path2)){
                        brls::Logger::debug("%s deleted", path2);
                        Mara::fs::DeleteDir(path2);
                    }

                    std::string nspgame = "exefs.nsp";
                    std::string nsppath = path + nspgame;
                    if(Mara::fs::copy_file(HBL_R0MFS_PATH, nsppath.c_str())) {
                        dialog->hide([dialog]() {
                            brls::Dialog* dialog_done = new brls::Dialog("main/installer/appletinstalldone/message"_i18n);

                            brls::GenericEvent::Callback yesCallback_done = [dialog_done](brls::View* view){
                                if (patchData->game_found){
                                    patchData->program->Launch();
                                    brls::Application::quit();
                                }
                            };

                            brls::GenericEvent::Callback noCallback_done = [dialog_done, dialog](brls::View* view) {
                                dialog_done->close();
                            };

                            dialog_done->addButton("main/installer/appletinstalldone/yes"_i18n, yesCallback_done);
                            dialog_done->addButton("main/installer/appletinstalldone/no"_i18n, noCallback_done);
                            dialog_done->setCancelable(true);
                            dialog_done->open();
                        });
                    }
                };

                dialog->addButton("main/installer/appletinstallwarning/yes"_i18n, yesCallback);
                dialog->addButton("main/installer/appletinstallwarning/no"_i18n, noCallback);

                dialog->setCancelable(true);
                dialog->open();
            });

            auto* appletlaunch = new brls::ListItem("Lanzar Juego", "");
            appletlaunch->getClickEvent()->subscribe([path](brls::View *view) {
                brls::Logger::debug("Lanzado juego...");


                std::string nspgame = "exefs.nsp";
                std::string nsppath = path + nspgame;

                if(R_SUCCEEDED(Mara::fs::checkFile(nsppath.c_str()))){
                    if (patchData->game_found){
                        patchData->program->Launch();
                        brls::Application::quit();
                    }
                } else {
                    brls::Application::notify("No has instalado todavía el parcheador, instala el parcheador para poder lanzar el juego desde Mara.");
                }
            });

            tablist->addView(appletinstall);
            tablist->addView(appletlaunch);
        }
    }

    brls::List *InstallerTab::GetTab() {
        return tablist;
    }
}