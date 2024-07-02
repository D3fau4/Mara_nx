#pragma once

#include <borealis.hpp>

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
