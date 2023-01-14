#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <switch.h>
#include <borealis.hpp>

#include "Program/Main.hpp"
#include "ui/MainActivity.hpp"

brls::audio_switch* audioSwitch;

// Main program entrypoint
int main(int argc, char* argv[])
{
    appletInitializeGamePlayRecording();
    nsInitialize();

    i18n::loadTranslations();

    brls::Logger::setLogLevel(brls::LogLevel::DEBUG);

    if (!brls::Application::init("main/title"_i18n))
    {
        brls::Logger::error("Unable to init Mara");
        return EXIT_FAILURE;
    }
    // Init audio
    audioSwitch = new brls::audio_switch();

    // Establece que se pueda salir de la app
    brls::Application::setGlobalQuit(true);
    auto win = new Mara::ui::MainActivity();
    brls::Application::pushView(win->GetView());

    while (brls::Application::mainLoop())
    {
        // Algo a ejecutar por cada refresco
    }

    // close audio
    audioSwitch->Close();

    return EXIT_SUCCESS;
}
