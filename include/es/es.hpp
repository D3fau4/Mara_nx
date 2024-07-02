#pragma once

#include "../types.h"
#include "tc/io/IStream.h"
#include <switch.h>
#include <borealis.hpp>

namespace Mara {
    class es {
    public:
        es();

        virtual ~es();

        Result CountPersonalizedTicket(s32 *out_count);
        Result CountCommonTicket(s32 *out_count);
        Result CountPrepurchaseRecord(s32 *out_count);
        Result ListCommonTicketRightsIds(s32 *out_entries_written, FsRightsId *out_ids, s32 count);
        Result ListPersonalizedTicketRightsIds(s32 *out_entries_written, FsRightsId *out_ids, s32 count);
        Result ListPrepurchaseRecordRightsIds(s32 *out_entries_written, FsRightsId *out_ids, s32 count);

        /* CUSTOM */
        bool isRightIdPurchased(u64 right_id);
        bool isRightIdPirated(u64 right_id);
        bool checkGameCardSig(u8 data[]);

        enum class TicketType {
            Common,
            Personalized,
        };
        union RightsId {
            struct {
                u64 app_id_be;
                u64 key_gen_be;
            };
            u8 id[0x10];

            inline u64 GetApplicationId() const {
                return __builtin_bswap64(this->app_id_be);
            }

            inline u8 GetKeyGeneration() const {
                return static_cast<u8>(__builtin_bswap64(this->key_gen_be));
            }
        };

        static_assert(sizeof(RightsId) == 0x10);
        enum class TicketSignature : u32 {
            Invalid,
            RSA_4096_SHA1 = 0x10000,
            RSA_2048_SHA1 = 0x10001,
            ECDSA_SHA1 = 0x10002,
            RSA_4096_SHA256 = 0x10003,
            RSA_2048_SHA256 = 0x10004,
            ECDSA_SHA256 = 0x10005,
            HMAC_SHA1_160 = 0x10006
        };

        inline constexpr static bool IsValidTicketSignature(const TicketSignature sig) {
            return (sig >= TicketSignature::RSA_4096_SHA1) && (sig <= TicketSignature::ECDSA_SHA256);
        }

        enum class TicketFlags : u8 {
            PreInstalled = BIT(0),
            Shared = BIT(1),
            AllContents = BIT(2),
            DeviceLinkIndependent = BIT(3),
            Temporary = BIT(4)
        };

        struct TicketData {
            u8 issuer[0x40];
            u8 title_key_block[0x100];
            u8 ticket_version;
            u8 title_key_type;
            u8 unk_1[0x2];
            u8 license_type;
            u8 master_key_gen;
            TicketFlags flags;
            u8 unk_2[0x8];
            u8 ticket_id[0x8];
            u8 device_id[0x8];
            es::RightsId rights_id;
            u8 account_id[0x4];
            u8 unk_3[0xC];

            std::string GetTitleKey() const;
        };
        static_assert(sizeof(TicketData) == 0x180);

        inline constexpr static u64 GetTicketSignatureDataSize(const TicketSignature sig) {
            switch(sig) {
                case TicketSignature::RSA_4096_SHA1:
                case TicketSignature::RSA_4096_SHA256: {
                    return 0x200 + 0x3C;
                }
                case TicketSignature::RSA_2048_SHA1:
                case TicketSignature::RSA_2048_SHA256: {
                    return 0x100 + 0x3C;
                }
                case TicketSignature::ECDSA_SHA1:
                case TicketSignature::ECDSA_SHA256: {
                    return 0x3C + 0x40;
                }
                case TicketSignature::HMAC_SHA1_160: {
                    return 0x14 + 0x28;
                }
                default: {
                    return 0;
                }
            }
        }

        inline constexpr static u64 GetTicketSignatureSize(const TicketSignature sig) {
            return sizeof(sig) + GetTicketSignatureDataSize(sig);
        }

        struct TicketFile {
            TicketSignature signature;
            u8 signature_data[0x300];
            TicketData data;
        };

    private:
        Service g_EsService;

        u8 InvalidCertSig[0x100] = {
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                0xFF, 0xFF, 0xFF, 0xFF
        };
    };

    void WriteToStream(tc::io::IStream &stream, const es::TicketFile &tik_file);
}