#pragma once

#include <borealis.hpp>

namespace Mara::ui {
    enum SplashMode {
        SplashMode_None,
        SplashMode_Applet,
        SplashMode_App,
    };

    class SplashScreen : public brls::View {
    public:
        SplashScreen(SplashMode splashMode);

        virtual ~SplashScreen();

        void draw(NVGcontext* vg, int x, int y, unsigned width, unsigned height, brls::Style* style, brls::FrameContext* ctx) override;
        void layout(NVGcontext* vg, brls::Style* style, brls::FontStash* stash) override;
    private:
        SplashMode splashMode;
        int miliseconds = 0;
        brls::Image* m_logo;
    };
}
