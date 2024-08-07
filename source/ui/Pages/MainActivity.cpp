﻿#include "ui/Pages/MainActivity.hpp"
#include "ns/ns.hpp"
#include "json.hpp"

namespace Mara::ui
{
    MainActivity::MainActivity()
    {
        brls::TabFrame* rootFrame = new brls::TabFrame();
        rootFrame->setTitle("main/title"_i18n);
        rootFrame->setIcon(BOREALIS_ASSET("icon/borealis.jpg"));

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
