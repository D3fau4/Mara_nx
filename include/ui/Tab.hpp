#pragma once

namespace Mara::ui {
    class Tab {
    public:
        virtual ~Tab() {};
        virtual brls::List* GetTab() = 0;
    };
}