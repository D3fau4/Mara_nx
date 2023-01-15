#include "ui/Pages/Installer/InstallerProgessPage.hpp"
#include "Helper.h"
#include "ns/ns.hpp"
#include "fs/fs.hpp"

namespace Mara::ui {

    InstallerProgessPage::InstallerProgessPage(brls::StagedAppletFrame *frame) : frame(frame) {
        // Label
        this->progressDisp = new brls::ProgressDisplay();
        this->progressDisp->setProgress(this->progressValue, 1000);
        this->progressDisp->setParent(this);
        this->label = new brls::Label(brls::LabelStyle::DIALOG, "installer/stage2/title"_i18n, true);
        this->label->setHorizontalAlign(NVG_ALIGN_CENTER);
        this->label->setParent(this);

        Result rc = romfsMountFromCurrentProcess(mountnamegame.c_str());
        if(R_SUCCEEDED(rc)){
            brls::Logger::debug("Romfs del juego montado correctamente");
            for (auto &title : Mara::ns::getAllTitles())
            {
                if(title.second->GetTitleID() == GAME_PID_USA || title.second->GetTitleID() == GAME_PID_EUR) {
                    brls::Logger::debug("Juego instalado: %016X", title.second->GetTitleID());
                    titlepid = title.second->GetTitleID();
                    break;
                }
            }

            rc = appletSetCpuBoostMode(ApmCpuBoostMode_FastLoad);
            if(R_SUCCEEDED(rc)){
                brls::Logger::info("Activado Overclock");
            }
        } else {
            brls::Logger::error("No se pudo montar el romfs del juego");
        }
    }

    InstallerProgessPage::~InstallerProgessPage() {
        delete this->progressDisp;
        delete this->label;
    }

    void InstallerProgessPage::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height,
                                    brls::Style *style, brls::FrameContext *ctx) {
        if (progressValue == std::size(patches)){
            appletSetCpuBoostMode(ApmCpuBoostMode_Normal);
            if (frame->isLastStage())
                brls::Application::popView();
            else
                frame->nextStage();
        }

        if(!this->running && this->progressValue <= std::size(patches) && !t.joinable()) {
            this->running = true;
            brls::Logger::debug("Start thread");

            t = std::thread(std::bind(&InstallerProgessPage::asyncPatch, this, this->progressValue));
        } else if(!this->running && t.joinable())
            t.join();

        this->progressDisp->setProgress(this->progressValue, std::size(patches));
        this->progressDisp->frame(ctx);
        this->label->frame(ctx);
    }

    void InstallerProgessPage::layout(NVGcontext *vg, brls::Style *style, brls::FontStash *stash) {
        this->label->setWidth(roundf((float)this->width * style->CrashFrame.labelWidth));
        this->label->invalidate(true);

        this->label->setBoundaries(
                this->x + this->width / 2 - this->label->getWidth() / 2,
                this->y + (this->height - style->AppletFrame.footerHeight) / 2,
                this->label->getWidth(),
                this->label->getHeight());

        this->progressDisp->setBoundaries(
                this->x + this->width / 2 - style->CrashFrame.buttonWidth,
                this->y + this->height / 2,
                style->CrashFrame.buttonWidth * 2,
                style->CrashFrame.buttonHeight);
    }

    void InstallerProgessPage::willAppear(bool resetState) {
        this->progressDisp->willAppear(resetState);
    }

    void InstallerProgessPage::willDisappear(bool resetState) {
        this->progressDisp->willDisappear(resetState);
    }

    void InstallerProgessPage::asyncPatch(int i) {
        char path[255];
        sprintf(path, "%s:/%s", mountnamegame.c_str(), ori_files[i].c_str());
        std::string orifile = path;
        sprintf(path, "%s:/atmosphere/contents/%016lX/romfs/%s", sdmountname.c_str(), titlepid, ori_files[i].c_str());
        std::string outfile = path;
        sprintf(path, "%s%s", mountmaraname.c_str(), patches[i].c_str());
        std::string patchfile = path;

        brls::Logger::debug(orifile);
        brls::Logger::debug(patchfile);
        brls::Logger::debug(outfile);

        if(Mara::fs::DeleteFile(outfile));

        std::size_t last_slash_pos = outfile.find_last_of("/");
        std::string folder = outfile.substr(0, last_slash_pos);
        if(Mara::fs::createdir(folder)){
            brls::Logger::error("No se pudo crear la carpeta");
        }

        int res = patch(orifile, outfile, patchfile);
        if (res == 0)
            brls::Logger::info("Patched Successful!");
        else
            brls::Logger::error("Patched Failed!: %i", res);

        this->progressValue++;
        this->running = false;
    }
}