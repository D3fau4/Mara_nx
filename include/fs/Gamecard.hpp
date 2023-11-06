#pragma once

#include "types.h"
#include "fs_ext.h"

namespace Mara::fs {

#define GAMECARD_HFS0_MAGIC                     0x48465330              /* "HFS0". */

#define GAMECARD_READ_BUFFER_SIZE               0x800000                /* 8 MiB. */

#define GAMECARD_UNUSED_AREA_BLOCK_SIZE         0x24
#define GAMECARD_UNUSED_AREA_SIZE(x)            (((x) / GAMECARD_PAGE_SIZE) * GAMECARD_UNUSED_AREA_BLOCK_SIZE)

#define GAMECARD_STORAGE_AREA_NAME(x)           ((x) == GameCardStorageArea_Normal ? "normal" : ((x) == GameCardStorageArea_Secure ? "secure" : "none"))

#define GAMECARD_HFS_PARTITION_NAME_INDEX(x)    ((x) - 1)

#define LAFW_MAGIC                              0x4C414657              /* "LAFW". */

    typedef enum {
        GameCardStorageArea_None   = 0,
        GameCardStorageArea_Normal = 1,
        GameCardStorageArea_Secure = 2
    } GameCardStorageArea;

    typedef enum {
        GameCardCapacity_1GiB  = BIT_LONG(30),
        GameCardCapacity_2GiB  = BIT_LONG(31),
        GameCardCapacity_4GiB  = BIT_LONG(32),
        GameCardCapacity_8GiB  = BIT_LONG(33),
        GameCardCapacity_16GiB = BIT_LONG(34),
        GameCardCapacity_32GiB = BIT_LONG(35)
    } GameCardCapacity;

    typedef struct {
        u8 kek_index          : 4;  ///< GameCardKekIndex.
        u8 titlekey_dec_index : 4;
    } GameCardKeyIndex;

    typedef struct {
        u64 fw_version;             ///< GameCardFwVersion.
        u32 acc_ctrl_1;             ///< GameCardAccCtrl1.
        u32 wait_1_time_read;       ///< Always 0x1388.
        u32 wait_2_time_read;       ///< Always 0.
        u32 wait_1_time_write;      ///< Always 0.
        u32 wait_2_time_write;      ///< Always 0.
        SdkAddOnVersion fw_mode;    ///< Current SdkAddOnVersion.
        Version upp_version;        ///< Bundled system update version.
        u8 compatibility_type;      ///< GameCardCompatibilityType.
        u8 reserved_1[0x3];
        u64 upp_hash;               ///< SHA-256 (?) checksum for the update partition. The exact way it's calculated is currently unknown.
        u64 upp_id;                 ///< Must match GAMECARD_UPDATE_TID.
        u8 reserved_2[0x38];
    } GameCardInfo;

    /// Placed after the `GameCardKeyArea` section.
    typedef struct {
        u8 signature[0x100];                            ///< RSA-2048-PSS with SHA-256 signature over the rest of the header.
        u32 magic;                                      ///< "HEAD".
        u32 rom_area_start_page_address;                ///< Expressed in GAMECARD_PAGE_SIZE units.
        u32 backup_area_start_page_address;             ///< Always 0xFFFFFFFF.
        GameCardKeyIndex key_index;
        u8 rom_size;                                    ///< GameCardRomSize.
        u8 header_version;                              ///< Always 0.
        u8 flags;                                       ///< GameCardFlags.
        u64 package_id;                                 ///< Used for challenge-response authentication.
        u32 valid_data_end_address;                     ///< Expressed in GAMECARD_PAGE_SIZE units.
        u8 reserved[0x4];
        u8 card_info_iv[AES_128_KEY_SIZE];              ///< AES-128-CBC IV for the CardInfo area (reversed).
        u64 partition_fs_header_address;                ///< Root Hash File System header offset.
        u64 partition_fs_header_size;                   ///< Root Hash File System header size.
        u8 partition_fs_header_hash[SHA256_HASH_SIZE];
        u8 initial_data_hash[SHA256_HASH_SIZE];
        u32 sel_sec;                                    ///< GameCardSelSec.
        u32 sel_t1_key;                                 ///< Always 2.
        u32 sel_key;                                    ///< Always 0.
        u32 lim_area;                                   ///< Expressed in GAMECARD_PAGE_SIZE units.
        GameCardInfo card_info;
    } GameCardHeader;

    class Gamecard {
    public:
        GameCardHeader g_gameCardHeader = {0};

        Gamecard();
        virtual ~Gamecard();

        Result ReadHeader();

    private:
        FsDeviceOperator g_deviceOperator = {0};
        FsEventNotifier g_gameCardEventNotifier = {0};
        FsGameCardHandle g_gameCardHandle = {0};
        FsStorage g_gameCardStorage = {0};

        bool gamecardIsInserted(void);
    };
}