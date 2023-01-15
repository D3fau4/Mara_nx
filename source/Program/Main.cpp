#include <stdlib.h>

#include <switch.h>
#include <borealis.hpp>

#include "Program/Main.hpp"
#include "ui/Pages/SplashScreen.hpp"

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

    // Establece que se pueda salir de la app
    brls::Application::setGlobalQuit(true);
    auto splash = new Mara::ui::SplashScreen();
    brls::Application::pushView(splash);

    while (brls::Application::mainLoop())
    {
        // Algo a ejecutar por cada refresco
    }

    shutdownServices();

    return EXIT_SUCCESS;
}
