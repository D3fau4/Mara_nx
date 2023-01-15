#include "ui/Pages/SplashScreen.hpp"
#include "ui/Pages/MainActivity.hpp"
#include "ui/Pages/Installer/InstallerPage.hpp"
#include "ui/Pages/Installer/InstallerProgessPage.hpp"
#include "ui/Pages/Installer/InstallerFinalPage.hpp"

namespace Mara::ui {
    SplashScreen::SplashScreen(SplashMode splashMode) {
        this->splashMode = splashMode;
        switch (splashMode) {
            case SplashMode::SplashMode_Applet:
            default:
                m_logo = new brls::Image(BOREALIS_ASSET("img/splash_ts.png"));
                break;
            case SplashMode::SplashMode_App:
                m_logo = new brls::Image(BOREALIS_ASSET("img/splash_hack.png"));
                break;
        }
        m_logo->setParent(this);
    }

    SplashScreen::~SplashScreen() {
        delete m_logo;
    }

    void StartInstaller(){
        brls::StagedAppletFrame* stagedFrame = new brls::StagedAppletFrame();
        stagedFrame->setTitle("installer/title"_i18n);
        stagedFrame->addStage(new InstallerPage(stagedFrame, "installer/stage1/button"_i18n));
        stagedFrame->addStage(new InstallerProgessPage(stagedFrame));
        stagedFrame->addStage(new InstallerFinalPage(stagedFrame, "installer/stage3/button"_i18n));
        brls::Application::pushView(stagedFrame);
    }

    void StartApp(){
        // Start Main Activity
        auto pMainActivity = new Mara::ui::MainActivity();
        if(!pMainActivity->found){
            brls::Logger::error("Juego no encontrado.");
            brls::Application::crash("main/error/gamenotfound"_i18n);
            return;
        }
        brls::Application::pushView(pMainActivity->GetView(), brls::ViewAnimation::FADE);
    }

    void SplashScreen::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                            brls::FrameContext *ctx) {
        NVGcolor color = a(nvgRGB(0x7f, 0xc7, 0xc0));
        miliseconds++;
        if (color.a == 0.0f){
            return;
        }

        if(miliseconds == 150) {
            switch(this->splashMode){
                case SplashMode::SplashMode_App:
                    StartInstaller();
                    break;
                case SplashMode::SplashMode_Applet:
                default:
                    StartApp();
                    break;
            }
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
        this->m_logo->setBoundaries((1280 - 1280) / 2, (720 - 720) / 2, 1280, 720);
    }
}