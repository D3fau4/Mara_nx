#include <stdlib.h>

#include <switch.h>
#include <borealis.hpp>

#include "Program/Main.hpp"
#include "ui/Pages/SplashScreen.hpp"
#include "fs/fs.hpp"
#include "pm/pm.hpp"
#include "ns/ns.hpp"

void initServices(){
    nsInitialize();
    pmdmntInitialize();
}

void shutdownServices(){
    nsExit();
    pmdmntExit();
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    appletInitializeGamePlayRecording();

    initServices();

    i18n::loadTranslations();

    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

    if (!brls::Application::init("main/title"_i18n))
    {
        brls::Logger::error("Unable to init Mara");
        return EXIT_FAILURE;
    }

    brls::View* splash;

    if(Mara::pm::isInApplicationMode()) {

        // Borrar lanzador
        char path[255];
        for (auto &title : Mara::ns::getAllTitles())
        {
            if(title.second->GetTitleID() == GAME_PID_USA || title.second->GetTitleID() == GAME_PID_EUR) {
                sprintf(path, "sdmc:/atmosphere/contents/%016lX/exefs.nsp", title.second->GetTitleID());
                Mara::fs::DeleteFile(path);
            }
        }
        // Establece que no se pueda salir de la app
        brls::Application::setGlobalQuit(false);
        // Nuevo splash screen
        splash = new Mara::ui::SplashScreen(Mara::ui::SplashMode_App);
    } else {
        brls::Application::setGlobalQuit(true);
        splash = new Mara::ui::SplashScreen(Mara::ui::SplashMode_Applet);
    }

    brls::Application::pushView(splash);

    while (brls::Application::mainLoop())
    {
        // Algo a ejecutar por cada refresco
    }

    shutdownServices();

    return EXIT_SUCCESS;
}
