#include "crypto_backend.h"

#include <cstring>

#if !defined(RECSRMESH_CRYPTO_PSA) && !defined(RECSRMESH_CRYPTO_LEGACY)
#  if __has_include(<tf-psa-crypto/build_info.h>)
#    define RECSRMESH_CRYPTO_PSA 1
#  else
#    define RECSRMESH_CRYPTO_LEGACY 1
#  endif
#endif

#if defined(RECSRMESH_CRYPTO_PSA)
#  include <psa/crypto.h>
#else
#  include <mbedtls/aes.h>
#  include <mbedtls/ctr_drbg.h>
#  include <mbedtls/entropy.h>
#  include <mbedtls/sha256.h>
#endif

namespace csrmesh::crypto::backend {

#if defined(RECSRMESH_CRYPTO_PSA)

static bool psa_ready()
{
    // psa_crypto_init() is idempotent and cheap after the first call.
    return psa_crypto_init() == PSA_SUCCESS;
}

const char *name() { return "psa"; }

Error sha256(ByteSpan a, ByteSpan b, uint8_t out[SHA256_LEN])
{
    if (!psa_ready())
        return Error::Crypto;

    psa_hash_operation_t op = PSA_HASH_OPERATION_INIT;
    size_t out_len = 0;
    if (psa_hash_setup(&op, PSA_ALG_SHA_256) != PSA_SUCCESS)
        return Error::Crypto;
    if ((!a.empty() && psa_hash_update(&op, a.data(), a.size()) != PSA_SUCCESS) ||
        (!b.empty() && psa_hash_update(&op, b.data(), b.size()) != PSA_SUCCESS) ||
        psa_hash_finish(&op, out, SHA256_LEN, &out_len) != PSA_SUCCESS ||
        out_len != SHA256_LEN) {
        psa_hash_abort(&op);
        return Error::Crypto;
    }
    return Error::Ok;
}

Error aes128_ofb(const uint8_t key[16], uint8_t iv[16],
                 const uint8_t *in, uint8_t *out, size_t len)
{
    if (len == 0)
        return Error::Ok;
    if (!psa_ready())
        return Error::Crypto;

    psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
    psa_set_key_type(&attr, PSA_KEY_TYPE_AES);
    psa_set_key_bits(&attr, 128);
    psa_set_key_usage_flags(&attr, PSA_KEY_USAGE_ENCRYPT);
    psa_set_key_algorithm(&attr, PSA_ALG_ECB_NO_PADDING);

    psa_key_id_t kid = 0;
    if (psa_import_key(&attr, key, 16, &kid) != PSA_SUCCESS)
        return Error::Crypto;

    Error result = Error::Ok;
    uint8_t block[16];
    size_t used = 16;  // force a keystream block on the first byte
    for (size_t i = 0; i < len; i++) {
        if (used == 16) {
            size_t block_len = 0;
            if (psa_cipher_encrypt(kid, PSA_ALG_ECB_NO_PADDING, iv, 16,
                                   block, sizeof(block), &block_len) != PSA_SUCCESS ||
                block_len != 16) {
                result = Error::Crypto;
                break;
            }
            std::memcpy(iv, block, 16);
            used = 0;
        }
        out[i] = in[i] ^ iv[used++];
    }

    psa_destroy_key(kid);
    return result;
}

Error random_bytes(uint8_t *out, size_t len)
{
    if (!psa_ready())
        return Error::Crypto;
    return psa_generate_random(out, len) == PSA_SUCCESS ? Error::Ok : Error::Crypto;
}

#else  // legacy Mbed TLS API

const char *name() { return "legacy"; }

Error sha256(ByteSpan a, ByteSpan b, uint8_t out[SHA256_LEN])
{
    mbedtls_sha256_context sha;
    mbedtls_sha256_init(&sha);
    Error result = Error::Crypto;
    if (mbedtls_sha256_starts(&sha, 0) == 0 &&
        (a.empty() || mbedtls_sha256_update(&sha, a.data(), a.size()) == 0) &&
        (b.empty() || mbedtls_sha256_update(&sha, b.data(), b.size()) == 0) &&
        mbedtls_sha256_finish(&sha, out) == 0)
        result = Error::Ok;
    mbedtls_sha256_free(&sha);
    return result;
}

Error aes128_ofb(const uint8_t key[16], uint8_t iv[16],
                 const uint8_t *in, uint8_t *out, size_t len)
{
    if (len == 0)
        return Error::Ok;

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    if (mbedtls_aes_setkey_enc(&aes, key, 128) != 0) {
        mbedtls_aes_free(&aes);
        return Error::Crypto;
    }

    Error result = Error::Ok;
    uint8_t block[16];
    size_t used = 16;
    for (size_t i = 0; i < len; i++) {
        if (used == 16) {
            if (mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, iv, block) != 0) {
                result = Error::Crypto;
                break;
            }
            std::memcpy(iv, block, 16);
            used = 0;
        }
        out[i] = in[i] ^ iv[used++];
    }

    mbedtls_aes_free(&aes);
    return result;
}

Error random_bytes(uint8_t *out, size_t len)
{
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    mbedtls_entropy_init(&entropy);
    mbedtls_ctr_drbg_init(&ctr_drbg);

    static const char pers[] = "recsrmesh_rng";
    Error result = Error::Crypto;
    if (mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy,
                              reinterpret_cast<const uint8_t *>(pers),
                              sizeof(pers) - 1) == 0 &&
        mbedtls_ctr_drbg_random(&ctr_drbg, out, len) == 0)
        result = Error::Ok;

    mbedtls_ctr_drbg_free(&ctr_drbg);
    mbedtls_entropy_free(&entropy);
    return result;
}

#endif

}  // namespace csrmesh::crypto::backend
