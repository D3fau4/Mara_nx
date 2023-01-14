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
        brls::ListItem *cabeza = new brls::ListItem("main/translation/title"_i18n, "", "main/translation/subtitle"_i18n);
        mainlist->addView(cabeza);

        brls::Label* patchdescription = new brls::Label(
                brls::LabelStyle::REGULAR,
                "main/translation/translatedby"_i18n,
                true);

        patchdescription->setHorizontalAlign(NVG_ALIGN_CENTER);
        mainlist->addView(patchdescription);

        brls::Image* a = new brls::Image(BOREALIS_ASSET("img/transcene_logo.png"));
        a->setHeight(150);
        mainlist->addView(a);

        // Creditos
        // Romhacking
        mainlist->addView(new brls::Header("main/translation/romhackingtitle"_i18n));
        brls::ListItem *hotaru = new brls::ListItem("Hotaru", "", "Un buen tio.");
        hotaru->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(hotaru);

        // Traducción
        mainlist->addView(new brls::Header("main/translation/translatortitle"_i18n));
        brls::ListItem *Damon = new brls::ListItem("Damon", "", "No le va el jamón.");
        Damon->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(Damon);

        brls::ListItem *Gerwulf = new brls::ListItem("Gerwulf", "", "Tremendo pana.");
        Gerwulf->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(Gerwulf);

        brls::ListItem *osco = new brls::ListItem("Óscar73", "", "Disgaea 2 son los padres...");
        osco->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(osco);

        brls::ListItem *jota = new brls::ListItem("Shiryu", "", "Una J con patas");
        jota->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(jota);

        // Correción
        mainlist->addView(new brls::Header("main/translation/correctiontitle"_i18n));
        brls::ListItem *leeg = new brls::ListItem("Leeg", "", "Un maestro de las lenguas.");
        leeg->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(leeg);

        brls::ListItem *jota1 = new brls::ListItem("Shiryu", "", "Shiryu al cuadrado.");
        jota1->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(jota1);

        mainlist->addView(new brls::Header("main/translation/graphicstitle"_i18n));
        brls::ListItem *fox = new brls::ListItem("Nakufox", "", "Maestro de las artes.");
        fox->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        mainlist->addView(fox);

        rootFrame->addTab("main/creditstab"_i18n, mainlist);
        view = rootFrame;
    }

    brls::View* MainActivity::GetView()
    {
        return view;
    }
}
