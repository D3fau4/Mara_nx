//
// Created by raul2 on 21/01/2023.
//

#include "es/es.hpp"

namespace Mara {
    es::es(int securityLevel) {

        splEsInitialize();
        this->securityLevel = static_cast<SecurityLevel>(securityLevel);
    }

    es::~es() {
        splEsExit();
    }
}