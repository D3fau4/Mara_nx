//
// Created by raul2 on 04/02/2023.
//

#include "fs/Gamecard.hpp"
#include "borealis/logger.hpp"
#include <switch/services/fs.h>

namespace Mara::fs {
    Gamecard::Gamecard() {
        Result rc = fsOpenDeviceOperator(&g_deviceOperator);
        if(R_FAILED(rc)){
            brls::Logger::error("fsOpenDeviceOperator failed: {}", rc);
            return;
        }
        /* Retrieve gamecard detection kernel event. */
        rc = fsOpenGameCardDetectionEventNotifier(&g_gameCardEventNotifier);
        if (R_FAILED(rc))
        {
            brls::Logger::error("Error opening gamecard detection event notifier.");
            return;
        }
    }

    Gamecard::~Gamecard() {
    }

    bool Gamecard::gamecardIsInserted(void)
    {
        bool inserted = false;
        Result rc = fsDeviceOperatorIsGameCardInserted(&g_deviceOperator, &inserted);
        if (R_FAILED(rc)) return false;
        return (R_SUCCEEDED(rc) && inserted);
    }

    Result Gamecard::ReadHeader(){
        Result rc = fsDeviceOperatorGetGameCardHandle(&g_deviceOperator, &g_gameCardHandle);
        if(R_FAILED(rc)){
            //Pete
            return rc;
        }

        rc = fsOpenGameCardStorage(&g_gameCardStorage, &g_gameCardHandle, GameCardStorageArea_Normal - 1);
        if (R_FAILED(rc)){
            return rc;
        }

        rc = fsStorageRead(&g_gameCardStorage, 0, &g_gameCardHeader, sizeof(GameCardHeader));

        return rc;
    }
}