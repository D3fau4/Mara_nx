#include "ui/Pages/MainActivity.hpp"
#include "ns/ns.hpp"
#include "hos/savefs.hpp"
#include "ui/Tabs/AboutTab.hpp"
#include "ui/Tabs/InstallerTab.hpp"
#include "EsTikProcess.h"

namespace Mara::ui
{
    MainActivity::MainActivity()
    {
        brls::TabFrame* rootFrame = new brls::TabFrame();
        rootFrame->setTitle("main/title"_i18n);
        rootFrame->setIcon(BOREALIS_ASSET("icon/borealis.jpg"));

        this->found = false;
        for (auto &title : Mara::ns::getAllTitles())
        {
            if(title.second->GetTitleID() == patchData->program->GetTitleID()) {
                Mara::es::TicketFile tik = Mara::hos::ReadTicket(title.second->GetTitleID());
                brls::Logger::info("Juego encontrado");
                this->found  = true;
            }
        }

        rootFrame->addTab("main/creditstab"_i18n, (new AboutTab())->GetTab());
        rootFrame->addSeparator();
        rootFrame->addTab("main/installer/title"_i18n, (new InstallerTab())->GetTab());
        view = rootFrame;
    }

    brls::View* MainActivity::GetView()
    {
        return view;
    }
}
