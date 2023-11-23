#include "es/Security.hpp"
#include "Program/Main.hpp"

namespace Mara {

    Security::Security() {
        this->es = new Mara::es();

        this->gamecard = new Mara::fs::Gamecard();
    }

    Security::~Security() {
        delete es;
        delete gamecard;
    }

    bool Security::IsPirated() {

        if(es->isRightIdPurchased(patchData->program->GetTitleID()))
        {
            brls::Logger::info("Juego comprado encontrado");
            return false;
        }
        else if(es->isRightIdPirated(patchData->program->GetTitleID()))
        {
            brls::Logger::info("Juego pirateado encontrado");
            return true;
        }

        // TODO: Normalmente cuando se ejecute esta función sera cuando no detecte el juego instalado
        //  y si se ejecute el parcheador será porque tiene el cartucho insertado
        if(es->checkGameCardSig(gamecard->g_gameCardHeader.signature))
        {
            brls::Logger::info("Cartucho original");
            return false;
        }
        else
        {
            brls::Logger::error("Cartucho pirata");
            return true;
        }

        return false;
    }

    bool Security::IsLegal() {
        return !IsPirated();
    }
}