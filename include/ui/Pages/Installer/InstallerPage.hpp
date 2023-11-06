#pragma once

#include "borealis.hpp"
#include "Program/Main.hpp"
#include <math.h>

namespace Mara::ui{
    class InstallerPage : public brls::View {
    private:
        brls::Button* button;
        brls::Label* label;
    public:
        InstallerPage(brls::StagedAppletFrame* frame, std::string label);
        ~InstallerPage();

        void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;
        void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;
        brls::View* getDefaultFocus() override;
    };
}

