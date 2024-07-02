#pragma once
#include <string>
#include <vector>
#include <mbedtls/rsa.h>
#include <mbedtls/sha1.h>
#include <mbedtls/sha256.h>
#include <stdexcept>

namespace Mara::hos::Crypto {

    enum SignatureId
    {
        SIGN_ID_RSA4096_SHA1 = 0x10000,
        SIGN_ID_RSA2048_SHA1,
        SIGN_ID_ECDSA240_SHA1,
        SIGN_ID_RSA4096_SHA256,
        SIGN_ID_RSA2048_SHA256,
        SIGN_ID_ECDSA240_SHA256,
    };

    enum SignatureAlgo
    {
        SIGN_ALGO_RSA4096,
        SIGN_ALGO_RSA2048,
        SIGN_ALGO_ECDSA240
    };

    enum HashAlgo
    {
        HASH_ALGO_SHA1,
        HASH_ALGO_SHA256
    };

    static const size_t kEcdsaSigSize = 0x3C;

    static const std::string kRootIssuerStr = "Root";
    static const std::string kIdentDelimiter = "-";

    inline int getSignatureAlgo(int signature_id) {
        switch (signature_id) {
            case SIGN_ID_RSA2048_SHA1:
            case SIGN_ID_RSA2048_SHA256:
                return SIGN_ALGO_RSA2048;
            case SIGN_ID_ECDSA240_SHA1:
            case SIGN_ID_ECDSA240_SHA256:
                return SIGN_ALGO_ECDSA240;
            case SIGN_ID_RSA4096_SHA1:
            case SIGN_ID_RSA4096_SHA256:
            default:
                return SIGN_ALGO_RSA4096;
        }
    }

    void validateSignature(const std::string& issuer, int signature_id, const std::vector<unsigned char>& signature, const std::vector<unsigned char>& hash);
}