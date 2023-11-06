#pragma once

#include <borealis.hpp>
#include "../../Program/Main.hpp"
#include "ui/Tab.hpp"
#include "pm/pm.hpp"
#include "fs/fs.hpp"

namespace Mara {
    namespace ui {

        class InstallerTab : public Tab{
        private:
            brls::List *tablist;
        public:
            InstallerTab();

            brls::List *GetTab() override;
        };

    } // Mara
} // ui
