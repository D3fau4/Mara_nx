//
// Created by raul2 on 15/01/2023.
//

#include "ui/Pages/SplashScreen.hpp"
#include "ui/Pages/MainActivity.hpp"

namespace Mara::ui {
    SplashScreen::SplashScreen() {
        m_logo = new brls::Image(BOREALIS_ASSET("img/tradusquare_logo.png"));
        m_logo->setParent(this);
    }

    SplashScreen::~SplashScreen() {
        delete m_logo;
    }

    void SplashScreen::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                            brls::FrameContext *ctx) {
        NVGcolor color = a(nvgRGB(0x7f, 0xc7, 0xc0));
        miliseconds++;
        if (color.a == 0.0f){
            return;
        }

        if(miliseconds == 150) {
            // Start Main Activity
            auto pMainActivity = new Mara::ui::MainActivity();
            if(!pMainActivity->found){
                brls::Logger::error("Juego no encontrado.");
                brls::Application::crash("main/error/gamenotfound"_i18n);
                return;
            }
            brls::Application::pushView(pMainActivity->GetView(), brls::ViewAnimation::FADE);
        }

        nvgFillColor(vg, color);

        nvgBeginPath(vg);
        nvgRect(vg, 0, 0, width, (height - this->m_logo->getWidth()) / 2);
        nvgRect(vg, 0, (height + this->m_logo->getWidth()) / 2, width, (height - this->m_logo->getWidth()) / 2);
        nvgRect(vg, 0, (height - this->m_logo->getWidth()) / 2, (width - this->m_logo->getWidth()) / 2, this->m_logo->getWidth());
        nvgRect(vg, (width + this->m_logo->getWidth()) / 2, (height - this->m_logo->getWidth()) / 2, (width - this->m_logo->getWidth()) / 2, this->m_logo->getWidth());
        nvgFill(vg);

        this->m_logo->frame(ctx);
    }

    void SplashScreen::layout(NVGcontext *vg, brls::Style *style, brls::FontStash *stash) {
        this->m_logo->setBoundaries((1280 - 1024) / 2, (720 - 512) / 2, 1024, 512);
    }
}