#pragma once

#include <borealis.hpp>
#include "Program/Main.hpp"
#include "../Tab.hpp"

namespace Mara::ui{
    class AboutTab : public Tab {
    private:
        brls::List *tablist;
    public:
        AboutTab();

        brls::List* GetTab() override;

    };
}
