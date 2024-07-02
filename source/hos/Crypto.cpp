#include "hos/Crypto.hpp"

void validateSignature(const std::string& issuer, int signature_id, const std::vector<unsigned char>& signature, const std::vector<unsigned char>& hash) {
    int sign_algo = Mara::hos::Crypto::getSignatureAlgo(signature_id);
    bool sig_valid = false;
    mbedtls_rsa_context rsa_key;
    mbedtls_rsa_init(&rsa_key, MBEDTLS_RSA_PKCS_V15, 0);

    if (issuer == "Root") {

    }
}