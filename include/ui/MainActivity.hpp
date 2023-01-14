#pragma once

#include <borealis.hpp>
#include <switch.h>
#include "Program/Main.hpp"
#include "ui/elements/ProgramidListItem.hpp"

namespace Mara::ui
{
    class MainActivity
    {
    private:
        brls::View* view;
    public:
        MainActivity();

        brls::View* GetView();
    };
}
