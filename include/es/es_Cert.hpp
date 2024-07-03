#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cassert>
#include "../types.h"

namespace Mara::es {
    enum SigType : u32 {
        SigType_Rsa4096Sha1   = 65536,
        SigType_Rsa2048Sha1   = 65537,
        SigType_Ecc480Sha1    = 65538,
        SigType_Rsa4096Sha256 = 65539,
        SigType_Rsa2048Sha256 = 65540,
        SigType_Ecc480Sha256  = 65541,
        SigType_Hmac160Sha1   = 65542
    };

    enum PubKeyType : u32 {
        PubKeyType_Rsa4096 = 0,
        PubKeyType_Rsa2048 = 1,
        PubKeyType_Ecc480  = 2
    };

    struct SignatureBlock {
        SigType sig_type;
        union {
            struct {
                u8 sign[0x200];
                u8 reserved_1[0x3C];
            } rsa4096;
            struct {
                u8 sign[0x100];
                u8 reserved_1[0x3C];
            } rsa2048;
            struct {
                u8 sign[0x3C];
                u8 reserved_1[0x40];
            } ecc480;
            struct {
                u8 sign[0x14];
                u8 reserved_1[0x28];
            } hmac160;
        };
        char issuer[0x40];
    };

    struct CertHeader {
        PubKeyType pub_key_type;
        char subject[0x40]; // ServerId, DeviceId
        u32 date;
    };

    struct PublicKeyBlock {
        union {
            struct {
                u8 public_key[0x200];
                u32 public_exponent;
                u8 reserved_1[0x34];
            } rsa4096;
            struct {
                u8 public_key[0x100];
                u32 public_exponent;
                u8 reserved_1[0x34];
            } rsa2048;
            struct {
                u8 public_key[0x3C];
                u8 reserved_1[0x3C];
            } ecc480;
        };
    };

    struct Cert {
        SignatureBlock signature_block;
        CertHeader cert_header;
        PublicKeyBlock public_key_block;
    };

    inline bool IsValidCertificateSignature(const SignatureBlock& sig_block) {
        return (sig_block.sig_type >= SigType_Rsa4096Sha1 ) && (sig_block.sig_type <= SigType_Hmac160Sha1);
    }

    inline size_t GetCertificateSignatureSize(const SignatureBlock& sig_block) {
        switch (sig_block.sig_type) {
            case SigType_Rsa4096Sha1:
            case SigType_Rsa4096Sha256:
                return sizeof(sig_block.rsa4096);
            case SigType_Rsa2048Sha1:
            case SigType_Rsa2048Sha256:
                return sizeof(sig_block.rsa2048);
            case SigType_Ecc480Sha1:
            case SigType_Ecc480Sha256:
                return sizeof(sig_block.ecc480);
            case SigType_Hmac160Sha1:
                return sizeof(sig_block.hmac160);
            default:
                return 0;
        }
    }
}