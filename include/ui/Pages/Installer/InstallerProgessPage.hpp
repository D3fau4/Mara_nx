#include <borealis.hpp>
#include <math.h>
#include <thread>
#include <functional>
#include "Program/Main.hpp"

namespace Mara::ui {
    class InstallerProgessPage : public brls::View {
    private:
        brls::StagedAppletFrame* frame;
        brls::ProgressDisplay* progressDisp;
        brls::Label* label;
        brls::Label* Sublabel;

        int progressValue = 0;
        std::thread t;
        bool running = false;
        u64 titlepid;

    public:
        InstallerProgessPage(brls::StagedAppletFrame* frame);
        ~InstallerProgessPage();

        void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;
        void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;

        void willAppear(bool resetState = false) override;
        void willDisappear(bool resetState = false) override;

        void asyncPatch();
    };
}