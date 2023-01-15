#include "pm/pm.hpp"

namespace Mara::pm {
    bool isInApplicationMode() {
        AppletType at = appletGetAppletType();
        if (at != AppletType_Application && at != AppletType_SystemApplication)
            return false;

        return true;
    }

    bool isInAppletMode(){
        return !isInApplicationMode();
    }
}