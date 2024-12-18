#include "hos/Crypto.hpp"

namespace Mara::hos::Crypto {
    void generate_hash(const unsigned char* input, size_t input_len, uint8_t* output, Mara::es::TicketSignature hash_algo)
    {
        if (hash_algo == Mara::es::TicketSignature::ECDSA_SHA1 || hash_algo == Mara::es::TicketSignature::RSA_2048_SHA1 || hash_algo == Mara::es::TicketSignature::HMAC_SHA1_160)
        {
            mbedtls_sha1_context sha1_ctx;
            mbedtls_sha1_init(&sha1_ctx);
            mbedtls_sha1_starts(&sha1_ctx);
            mbedtls_sha1_update(&sha1_ctx, input, input_len);
            mbedtls_sha1_finish(&sha1_ctx, output);
            mbedtls_sha1_free(&sha1_ctx);
        }
        else if (hash_algo == Mara::es::TicketSignature::ECDSA_SHA256 || hash_algo == Mara::es::TicketSignature::RSA_2048_SHA256 || hash_algo == Mara::es::TicketSignature::RSA_4096_SHA256)
        {
            mbedtls_sha256_context sha256_ctx;
            mbedtls_sha256_init(&sha256_ctx);
            mbedtls_sha256_starts(&sha256_ctx, 0); // 0 for SHA-256, 1 for SHA-224
            mbedtls_sha256_update(&sha256_ctx, input, input_len);
            mbedtls_sha256_finish(&sha256_ctx, output);
            mbedtls_sha256_free(&sha256_ctx);
        }
        else
        {

        }
    }

    void validateSignature(const std::string& issuer, Mara::es::TicketSignature signature_id, const u8* signature, const u8* hash) {
        bool sig_valid = false;
        mbedtls_rsa_context rsa_key;
        mbedtls_rsa_init(&rsa_key, MBEDTLS_RSA_PKCS_V15, 0);

    }

    bool CheckTicket(const Mara::es::TicketFile tik, const Mara::es::Cert cert){
        uint8_t *ticket_hash = {};
        generate_hash(reinterpret_cast<const u8*>(&tik.data), 0x180, ticket_hash, tik.signature);
        validateSignature(std::string(reinterpret_cast<const char*>(tik.data.issuer)), tik.signature, tik.signature_data, ticket_hash);

        return true;
    }
}