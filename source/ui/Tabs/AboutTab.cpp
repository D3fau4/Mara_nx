//
// Created by raul2 on 15/01/2023.
//

#include "ui/Tabs/AboutTab.hpp"

namespace Mara::ui {
    AboutTab::AboutTab() {
        tablist = new brls::List();
        brls::ListItem *cabeza = new brls::ListItem("main/translation/title"_i18n, "", "main/translation/subtitle"_i18n);
        tablist->addView(cabeza);

        brls::Label* patchdescription = new brls::Label(
                brls::LabelStyle::REGULAR,
                "main/translation/translatedby"_i18n,
                true);

        patchdescription->setHorizontalAlign(NVG_ALIGN_CENTER);
        tablist->addView(patchdescription);

        brls::Image* a = new brls::Image(BOREALIS_ASSET("img/transcene_logo.png"));
        a->setHeight(150);
        tablist->addView(a);

        // Creditos
        // Romhacking
        tablist->addView(new brls::Header("main/translation/romhackingtitle"_i18n));
        brls::ListItem *hotaru = new brls::ListItem("Hotaru", "", "Un buen tio.");
        hotaru->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(hotaru);

        // Traducción
        tablist->addView(new brls::Header("main/translation/translatortitle"_i18n));
        brls::ListItem *Damon = new brls::ListItem("Damon", "", "No le va el jamón.");
        Damon->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(Damon);

        brls::ListItem *Gerwulf = new brls::ListItem("Gerwulf", "", "Tremendo pana.");
        Gerwulf->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(Gerwulf);

        brls::ListItem *osco = new brls::ListItem("Óscar73", "", "Disgaea 2 son los padres...");
        osco->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(osco);

        brls::ListItem *jota = new brls::ListItem("Shiryu", "", "Una J con patas");
        jota->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(jota);

        // Correción
        tablist->addView(new brls::Header("main/translation/correctiontitle"_i18n));
        brls::ListItem *leeg = new brls::ListItem("Leeg", "", "Un maestro de las lenguas.");
        leeg->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(leeg);

        brls::ListItem *jota1 = new brls::ListItem("Shiryu", "", "Shiryu al cuadrado.");
        jota1->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(jota1);

        tablist->addView(new brls::Header("main/translation/graphicstitle"_i18n));
        brls::ListItem *fox = new brls::ListItem("Nakufox", "", "Maestro de las artes.");
        fox->setThumbnail(BOREALIS_ASSET("icon/borealis.jpg"));
        tablist->addView(fox);
    }

    brls::List* AboutTab::GetTab() {
        return this->tablist;
    }
}