#pragma once

#include "helpers/PatchData.hpp"

namespace i18n = brls::i18n;	// for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

// Mount names (No tocar)
#define GAME_MOUNT_NAME "gromfs"
#define SDCARD_MOUNT_NAME "sdmc"
#define ROMFS_MOUNT_NAME BOREALIS_RESOURCES

// Configuración de rutas
#define HBL_R0MFS_PATH ROMFS_MOUNT_NAME "Patch/hbl.nsp"
#define ABOUT_TAB_INFO ROMFS_MOUNT_NAME "Patch/credits.json"
#define VERSIONS_FILE ROMFS_MOUNT_NAME "Patch/versions.json"
#define MARA_PATH SDCARD_MOUNT_NAME ":/switch/Mara_nx.nro"
#define PROGRAM_SPLASH_PATH "img/splash_ts.png"
#define GAME_SPLASH_PATH "img/splash_hack.png"

extern Mara::helpers::PatchData* patchData;
