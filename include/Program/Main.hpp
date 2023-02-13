#pragma once

namespace i18n = brls::i18n;	// for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

#define GAME_MOUNT_NAME "gromfs"
#define GAME_PID_USA 0x010080B00AD66000 // UNDERTALE
#define GAME_PID_EUR 0x010080B00AD66000 // UNDERTALE
#define HBL_GAME "romfs:/Patch/hbl.nsp"

const std::string ori_files[1] = {
        "game.win",
};

const std::string patches[1] = {
        "Patch/game.win.xdelta",
};