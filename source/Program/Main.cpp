#include <stdlib.h>

#include <switch.h>
#include <borealis.hpp>

#include "Program/Main.hpp"
#include "ui/Pages/SplashScreen.hpp"
#include "fs/fs.hpp"
#include "helpers/Helper.hpp"
#include "pm/pm.hpp"
#include "ns/ns.hpp"
#ifdef CHECK_SIGNATURE
#include "es/es.hpp"
#include "fs/Gamecard.hpp"
#endif

void initServices(){
    fsInitialize();
    nsInitialize();
    pmdmntInitialize();
    apmInitialize();
    smInitialize();
}

void shutdownServices(){
    fsExit();
    nsExit();
    pmdmntExit();
    apmExit();
    smExit();
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    appletInitializeGamePlayRecording();

    initServices();
    Helper::initializeConstants();

    i18n::loadTranslations();

    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

    if (!brls::Application::init("main/title"_i18n))
    {
        brls::Logger::error("Unable to init Mara");
        return EXIT_FAILURE;
    }

    brls::View* splash;

#ifdef CHECK_SIGNATURE

    Mara::es *es = new Mara::es(Mara::es::SecurityLevel::SecurityLevel_Full);

    Mara::fs::Gamecard *gamecard = new Mara::fs::Gamecard();

    Result rc = gamecard->ReadHeader();
    if(R_FAILED(rc)){
        brls::Logger::error("Unable to read header");
    } else if(R_SUCCEEDED(rc)){
        brls::Logger::info("Cabecera del cartucho almacenado correctamente.");
    }

    if(es->checkGameCardSig(gamecard->g_gameCardHeader.signature))
        brls::Logger::info("Cartucho original");
    else
        brls::Logger::error("Cartucho pirata");

    if(es->isRightIdPurchased(Helper::program_id))
        brls::Logger::info("Juego comprado encontrado");
    else if(es->isRightIdPirated(Helper::program_id))
        brls::Logger::info("Juego pirateado encontrado");
    else
        brls::Logger::info("Ticket del juego no encontrado puede ser un cartucho.");

#endif


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
