#include "ui/Pages/Installer/InstallerFinalPage.hpp"
#include "ns/ns.hpp"

namespace Mara::ui {

    InstallerFinalPage::InstallerFinalPage(brls::StagedAppletFrame *frame, std::string label) {
        // Label
        this->button = (new brls::Button(brls::ButtonStyle::BORDERLESS))->setLabel(label);//->setImage(BOREALIS_ASSET("icon/borealis.jpg"));
        this->button->setParent(this);
        this->button->getClickEvent()->subscribe([frame](View* view) {
            if (frame->isLastStage()){
                for (auto &title : Mara::ns::getAllTitles())
                {
                    if(title.second->GetTitleID() == GAME_PID_USA || title.second->GetTitleID() == GAME_PID_EUR) {
                        title.second->Launch();
                        brls::Application::quit();
                        break;
                    }
                }
            }
            else
                frame->nextStage();
        });
        this->label = new brls::Label(brls::LabelStyle::DIALOG, "installer/stage3/title"_i18n, true);
        this->label->setHorizontalAlign(NVG_ALIGN_CENTER);
        this->label->setParent(this);
    }

    InstallerFinalPage::~InstallerFinalPage() {
        delete this->button;
        delete this->label;
    }

    void InstallerFinalPage::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                             brls::FrameContext *ctx) {
        this->label->frame(ctx);
        this->button->frame(ctx);
    }

    void InstallerFinalPage::layout(NVGcontext *vg, brls::Style *style, brls::FontStash *stash) {
        this->label->setWidth(roundf((float)this->width * style->CrashFrame.labelWidth));
        this->label->invalidate(true);

        this->label->setBoundaries(
                this->x + this->width / 2 - this->label->getWidth() / 2,
                this->y + (this->height - (style->AppletFrame.footerHeight + 55 )) / 2,
                this->label->getWidth(),
                this->label->getHeight());

        this->button->setBoundaries(
                this->x + this->width / 2 - style->CrashFrame.buttonWidth / 2,
                this->y + this->height / 2 + style->CrashFrame.buttonHeight,
                style->CrashFrame.buttonWidth,
                style->CrashFrame.buttonHeight);
        this->button->invalidate();
    }

    brls::View *InstallerFinalPage::getDefaultFocus() {
        return this->button;
    }
}