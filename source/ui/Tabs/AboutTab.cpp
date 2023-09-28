#include "ui/Tabs/AboutTab.hpp"

namespace Mara::ui {
    AboutTab::AboutTab() {
        tablist = new brls::List();
        brls::ListItem *cabeza = new brls::ListItem("main/translation/title"_i18n, "", "main/translation/subtitle"_i18n);
        tablist->addView(cabeza);

        brls::Label* Portdescription = new brls::Label(
                brls::LabelStyle::REGULAR,
                "main/translation/translatedby"_i18n,
                true);

        Portdescription->setHorizontalAlign(NVG_ALIGN_CENTER);
        tablist->addView(Portdescription);

        brls::Image* a = new brls::Image(BOREALIS_ASSET("img/utslogo.png"));
        a->setHeight(50);
        tablist->addView(a);

        brls::Label* patchdescription = new brls::Label(
                brls::LabelStyle::REGULAR,
                "Traducido por:",
                true);

        patchdescription->setHorizontalAlign(NVG_ALIGN_CENTER);
        tablist->addView(patchdescription);

        brls::Image* b = new brls::Image(BOREALIS_ASSET("img/utslogo2.png"));
        b->setHeight(50);
        tablist->addView(b);

        // Creditos

        // Port
        tablist->addView(new brls::Header("Port"));
        brls::ListItem *d3fau4 = new brls::ListItem("D3fau4", "", "");
        d3fau4->setThumbnail(BOREALIS_ASSET("img/d3fau4.png"));
        tablist->addView(d3fau4);

        // Team original
        tablist->addView(new brls::Header("Undertale-Spanish (Creadores de la traducción)"));
        // Romhacking
        tablist->addView(new brls::Header("main/translation/romhackingtitle"_i18n));
        brls::ListItem *hotaru = new brls::ListItem("Alex WaveDiver", "", "");
        hotaru->setThumbnail(BOREALIS_ASSET("img/alex.png"));
        tablist->addView(hotaru);

        // Traducción
        tablist->addView(new brls::Header("main/translation/translatortitle"_i18n));
        brls::ListItem *Damon = new brls::ListItem("Fandango", "", "");
        Damon->setThumbnail(BOREALIS_ASSET("img/avatar2.png"));
        tablist->addView(Damon);

        brls::ListItem *Gerwulf = new brls::ListItem("Kiro", "", "");
        Gerwulf->setThumbnail(BOREALIS_ASSET("img/avatar3.png"));
        tablist->addView(Gerwulf);

        brls::ListItem *Saku = new brls::ListItem("Saku", "", "");
        Saku->setThumbnail(BOREALIS_ASSET("img/Saku.png"));
        tablist->addView(Saku);
    }

    brls::List* AboutTab::GetTab() {
        return this->tablist;
    }
}