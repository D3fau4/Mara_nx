#include "ui/MainActivity.hpp"
#include "ns/ns.hpp"

namespace Mara::ui
{
    MainActivity::MainActivity()
    {
        brls::TabFrame* rootFrame = new brls::TabFrame();
        rootFrame->setTitle("main/title"_i18n);
        rootFrame->setIcon(BOREALIS_ASSET("icon/borealis.jpg"));

        brls::List *mainlist = new brls::List();
        this->found = false;
        for (auto &title : Mara::ns::getAllTitles())
        {
            if(title.second->GetTitleID() == GAME_PID) {
                brls::Logger::info("Juego encontrado");
                this->found  = true;
            }
        }

        brls::Image* a = new brls::Image(BOREALIS_ASSET("img/transcene_logo.png"));
        a->setHeight(150);
        mainlist->addView(a);

        brls::Label* patchdescription = new brls::Label(
                brls::LabelStyle::DESCRIPTION,
                "TEXTO TEXTO 1TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO TEXTO ",
                true);
        patchdescription->setHorizontalAlign(NVG_ALIGN_CENTER);
        mainlist->addView(patchdescription);
        rootFrame->addTab("main/maintab"_i18n, mainlist);
        view = rootFrame;
    }

    brls::View* MainActivity::GetView()
    {
        return view;
    }
}
