#pragma once

#include "es/es.hpp"
#include "fs/Gamecard.hpp"

namespace Mara {
    class Security {
    public:
        Security();
        virtual ~Security();

        bool IsPirated();
        bool IsLegal();

    private:
        Mara::es* es;
        Mara::fs::Gamecard* gamecard;
    };
}
