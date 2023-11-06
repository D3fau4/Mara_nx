#pragma once

#include "helpers/PatchData.hpp"

namespace i18n = brls::i18n;	// for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

// Mount names (No tocar)
#define GAME_MOUNT_NAME "gromfs"
#define SDCARD_MOUNT_NAME "sdmc"
#define ROMFS_MOUNT_NAME BOREALIS_RESOURCES

// Title IDs
#define GAME_PID_USA 0x0100CA9014A04000 // .Hack G.U Title id (USA)
#define GAME_PID_EUR 0x0100CA9014A04000 // .Hack G.U Title id (EUR)
#define GAME_PID_JP 0x0100BA9014A02000 // .Hack G.U Title id (EUR)

// Configuración de rutas
#define HBL_R0MFS_PATH ROMFS_MOUNT_NAME "Patch/hbl.nsp"
#define ABOUT_TAB_INFO ROMFS_MOUNT_NAME "Patch/credits.json"
#define VERSIONS_FILE ROMFS_MOUNT_NAME "Patch/version.json"
#define MARA_PATH SDCARD_MOUNT_NAME ":/switch/Mara_nx.nro"
#define PROGRAM_SPLASH_PATH "img/splash_ts.png"
#define GAME_SPLASH_PATH "img/splash_hack.png"

extern Mara::PatchData* patchData;

// Rutas de los archivos originales
const std::string ori_files[3] = {
        "cpkj/hackGU_cmn_a_nx.cpk",
        "cpkj/hackGU_cmn_i.cpk",
        "cpkj/hackGU_cmn_i_nx.cpk"
};

// Rutas de los xdelta
const std::string patches[3] = {
        "Patch/hackGU_cmn_a_nx.cpk.xdelta",
        "Patch/hackGU_cmn_i.cpk.xdelta",
        "Patch/hackGU_cmn_i_nx.cpk.xdelta"
};