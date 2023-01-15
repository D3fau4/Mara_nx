#pragma once

#include "borealis.hpp"
#include "Program/Main.hpp"
#include <math.h>

namespace Mara::ui{
    class InstallerFinalPage : public brls::View {
    private:
        brls::Button* button;
        brls::Label* label;
    public:
        InstallerFinalPage(brls::StagedAppletFrame* frame, std::string label);
        ~InstallerFinalPage();

        void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;
        void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;
        brls::View* getDefaultFocus() override;
    };
}

