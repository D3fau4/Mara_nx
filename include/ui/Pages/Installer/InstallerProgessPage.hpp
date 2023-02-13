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

        int progressValue = 0;
        std::thread t;
        bool running = false;
        u64 titlepid;

        const std::string mountnamegame = GAME_MOUNT_NAME;
        const std::string mountmaraname = BOREALIS_RESOURCES;
        const std::string sdmountname = "sdmc";

    public:
        InstallerProgessPage(brls::StagedAppletFrame* frame);
        ~InstallerProgessPage();

        void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;
        void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;

        void willAppear(bool resetState = false) override;
        void willDisappear(bool resetState = false) override;

        void asyncPatch(int i);
    };
}