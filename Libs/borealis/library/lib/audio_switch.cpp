//
// Created by raul2 on 14/01/2023.
//

#include <borealis/audio_switch.hpp>
#include <borealis/logger.hpp>

namespace brls {

    PLSR_RC audio_switch::_Init() {
        // Initialize our player using the default configuration
        PLSR_RC_TRY(plsrPlayerInit());

        // Mount Qlaunch
        romfsMountDataStorageFromProgram(QLAUNCH_PID, QLAUNCH_MOUNT_POINT);

        char bfsarPath[29];
        // Open the sound archive from qlaunch sound archive
        sprintf(bfsarPath, "%s:%s", QLAUNCH_MOUNT_POINT, BFSAR_PATH);
        PLSR_RC_TRY(plsrBFSAROpen(bfsarPath, &this->qlaunchBfsar));
        Logger::info("Qlaunch BFSAR mounted at: %s", bfsarPath);

        return PLSR_RC_OK;
    }

    audio_switch::audio_switch() {
        // Init the sounds array
        for (size_t sound = 0; sound < _SOUND_MAX; sound++)
            this->sounds[sound] = PLSR_PLAYER_INVALID_SOUND;

        _Init();
    }

    audio_switch::~audio_switch() {

    }

    bool audio_switch::Close(){
        for (size_t sound = 0; sound < _SOUND_MAX; sound++)
            if(this->sounds[sound] != PLSR_PLAYER_INVALID_SOUND){
                plsrPlayerFree(this->sounds[sound]);
                Logger::info("Sound unloaded: %i", sound);
            }


        // Close the archive
        plsrBFSARClose(&this->qlaunchBfsar);

        // De-initialize our player
        plsrPlayerExit();

        romfsUnmount(QLAUNCH_MOUNT_POINT);

        return true;
    }

    bool audio_switch::load(enum Sound sound) {

        if (sound == SOUND_NONE)
            return false;

        if (this->sounds[sound] != PLSR_PLAYER_INVALID_SOUND)
            return false;

        std::string soundName = SOUNDS_MAP[sound];

        if (soundName == "")
            return false; // unimplemented sound

        Logger::debug("Loading sound {}: {}", sound, soundName);

        PLSR_RC rc = plsrPlayerLoadSoundByName(&this->qlaunchBfsar, soundName.c_str(), &this->sounds[sound]);
        if (!PLSR_RC_SUCCEEDED(rc))
        {
            Logger::warning("Unable to load sound {}: {:#x}", soundName, rc);
            this->sounds[sound] = PLSR_PLAYER_INVALID_SOUND;
            return false;
        }

        return true;
    }

    bool audio_switch::play(enum Sound sound, float pitch) {

        if (sound == SOUND_NONE)
            return true;

        if (this->sounds[sound] == PLSR_PLAYER_INVALID_SOUND)
        {
            if (!this->load(sound))
                return false;
        }

        auto rc = plsrPlayerPlay(this->sounds[sound]);
        if(PLSR_RC_FAILED(rc))
            return false;

        return true;
    }
}
