#pragma once

#include <borealis.hpp>
#include <switch.h>
#include "Program/Main.hpp"
#include "ui/elements/ProgramidListItem.hpp"
#include "ui/Tabs/AboutTab.hpp"
#include "ui/Tabs/InstallerTab.hpp"

namespace Mara::ui
{
    class MainActivity
    {
    private:
        brls::View* view;
    public:
        MainActivity();

        brls::View* GetView();
        bool found;
    };
}
