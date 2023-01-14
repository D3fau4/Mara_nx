#pragma once

#include <switch.h>
#include <pulsar.h>
#include <borealis/audio.hpp>
#include <string>

#define QLAUNCH_PID 0x0100000000001000
#define QLAUNCH_MOUNT_POINT "qlaunch"
#define BFSAR_PATH "/sound/qlaunch.bfsar"

namespace brls {
    class audio_switch : public AudioPlayer {
    public:
        virtual ~audio_switch();
        audio_switch();

        bool load(Sound sound) override;
        bool play(Sound sound, float pitch = 1) override;
        bool Close();
    private:

        const std::string SOUNDS_MAP[_SOUND_MAX] = {
                "", // SOUND_NONE
                "SeBtnFocus", // SOUND_FOCUS_CHANGE
                "SeKeyErrorCursor", // SOUND_FOCUS_ERROR
                "SeBtnDecide", // SOUND_CLICK
                "SeFooterDecideFinish", // SOUND_BACK
                "SeNaviFocus", // SOUND_FOCUS_SIDEBAR
                "SeKeyError", // SOUND_CLICK_ERROR
                "SeUnlockKeyZR", // SOUND_HONK
                "SeNaviDecide", // SOUND_CLICK_SIDEBAR
                "SeTouchUnfocus", // SOUND_TOUCH_UNFOCUS
                "SeTouch", // SOUND_TOUCH
                "SeSliderTickOver", // SOUND_SLIDER_TICK
                "SeSliderRelease" // SOUND_SLIDER_RELEASE
        };

        PLSR_BFSAR qlaunchBfsar;
        PLSR_PlayerSoundId sounds[_SOUND_MAX];

        PLSR_RC _Init();
    };
}

