#include "ui/Pages/Installer/InstallerProgessPage.hpp"
#include "Helper.h"
#include "ns/ns.hpp"
#include "fs/fs.hpp"
#include "helpers/StringsUtils.hpp"

namespace Mara::ui {

    InstallerProgessPage::InstallerProgessPage(brls::StagedAppletFrame *frame) : frame(frame) {
        // Label
        this->progressDisp = new brls::ProgressDisplay();
        this->progressDisp->setProgress(this->progressValue, 1000);
        this->progressDisp->setParent(this);
        this->label = new brls::Label(brls::LabelStyle::DIALOG, "installer/stage2/title"_i18n, true);
        this->label->setHorizontalAlign(NVG_ALIGN_CENTER);
        this->label->setParent(this);

        this->Sublabel = new brls::Label(brls::LabelStyle::DESCRIPTION, "installer/stage2/warning"_i18n, true);
        this->Sublabel->setHorizontalAlign(NVG_ALIGN_CENTER);
        this->Sublabel->setParent(this);

        // Montar romfs del programa en el que se esta ejecutando
        Result rc = romfsMountFromCurrentProcess(GAME_MOUNT_NAME);
        if(R_SUCCEEDED(rc)){
            brls::Logger::debug("Romfs del juego montado correctamente");
            if (patchData->game_found) {
                brls::Logger::debug("Juego instalado: %016X", patchData->program->GetTitleID());
                titlepid = patchData->program->GetTitleID();
            }
            // Dar overclock a la CPU para poder parchear mas rapido
            rc = appletSetCpuBoostMode(ApmCpuBoostMode_FastLoad);
            if(R_SUCCEEDED(rc)){
                brls::Logger::info("Activado Overclock");
            }
        } else {
            brls::Logger::error("No se pudo montar el romfs del juego");
            brls::Application::crash("No se pudo montar el romfs del juego");
        }
    }

    InstallerProgessPage::~InstallerProgessPage() {
        delete this->progressDisp;
        delete this->label;
        delete this->Sublabel;
    }

    void InstallerProgessPage::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height,
                                    brls::Style *style, brls::FrameContext *ctx) {
        if (progressValue == patchData->patch_files.size()){
            // Volver la cpu a su estado normal
            appletSetCpuBoostMode(ApmCpuBoostMode_Normal);
            if (frame->isLastStage())
                brls::Application::popView();
            else
                frame->nextStage();
        }

        if(!this->running && this->progressValue <= patchData->patch_files.size() && !t.joinable()) {
            this->running = true;
            brls::Logger::debug("Iniciar hilo");

            t = std::thread(std::bind(&InstallerProgessPage::asyncPatch, this));
        } else if(!this->running && t.joinable())
            t.join();

        this->progressDisp->setProgress(this->progressValue, patchData->patch_files.size());
        this->progressDisp->frame(ctx);
        this->label->frame(ctx);
        this->Sublabel->frame(ctx);
    }

    void InstallerProgessPage::layout(NVGcontext *vg, brls::Style *style, brls::FontStash *stash) {
        this->label->setWidth(roundf((float)this->width * style->CrashFrame.labelWidth));
        this->label->invalidate(true);

        this->label->setBoundaries(
                this->x + this->width / 2 - this->label->getWidth() / 2,
                this->y + ((this->height - 50 ) - style->AppletFrame.footerHeight) / 2,
                this->label->getWidth(),
                this->label->getHeight());

        this->Sublabel->setWidth(roundf((float)this->width * style->CrashFrame.labelWidth));
        this->Sublabel->invalidate(true);

        this->Sublabel->setBoundaries(
                this->x + this->width / 2 - this->Sublabel->getWidth() / 2,
                this->y + ((this->height + 20) - style->AppletFrame.footerHeight) / 2,
                this->Sublabel->getWidth(),
                this->Sublabel->getHeight());

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

    void InstallerProgessPage::asyncPatch() {
        char path[255];
        sprintf(path, "%s:/%s", GAME_MOUNT_NAME, Mara::helpers::String::replace(patchData->patch_files[this->progressValue], ".xdelta", "").c_str());
        std::string orifile = path;
        sprintf(path, "%s:/atmosphere/contents/%016lX/romfs/%s", SDCARD_MOUNT_NAME, titlepid,
                Mara::helpers::String::replace(patchData->patch_files[this->progressValue], std::string(".xdelta"),  std::string()).c_str());
        std::string outfile = path;
        sprintf(path, "%s%s%s", ROMFS_MOUNT_NAME, patchData->base_path.c_str(), patchData->patch_files[this->progressValue].c_str());
        std::string patchfile = path;

        brls::Logger::debug("Ruta archivo original: %s", orifile.c_str());
        brls::Logger::debug("Ruta del parche: %s", patchfile.c_str());
        brls::Logger::debug("Ruta de salida: %s", outfile.c_str());

        // Elimina archivos antiguos
        Mara::fs::DeleteFile(outfile);

        std::size_t last_slash_pos = outfile.find_last_of("/");
        std::string folder = outfile.substr(0, last_slash_pos);
        if(Mara::fs::createdir(folder)) {
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