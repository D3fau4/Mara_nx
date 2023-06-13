#pragma once

namespace i18n = brls::i18n;	// for loadTranslations() and getStr()
using namespace i18n::literals; // for _i18n

#define GAME_MOUNT_NAME "gromfs"
#define GAME_PID_USA 0x0100BA9014A02000 // .Hack G.U Title id (USA)
#define GAME_PID_EUR 0x0100CA9014A04000 // .Hack G.U Title id (EUR)
#define HBL_GAME "romfs:/Patch/hbl.nsp"
#define SDCARD_MOUNT_NAME "sdmc"

const std::string ori_files[3] = {
        "cpkj/hackGU_cmn_a_nx.cpk",
        "cpkj/hackGU_cmn_i.cpk",
        "cpkj/hackGU_cmn_i_nx.cpk"
};

const std::string patches[3] = {
        "Patch/hackGU_cmn_a_nx.cpk.xdelta",
        "Patch/hackGU_cmn_i.cpk.xdelta",
        "Patch/hackGU_cmn_i_nx.cpk.xdelta"
};