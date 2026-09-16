#pragma once
// Thin crypto backend so the protocol code does not depend on a specific
// Mbed TLS generation.
//
//   * Mbed TLS 4.x / TF-PSA-Crypto (ESP-IDF 6.x): PSA Crypto API only.
//   * Mbed TLS 2.x/3.x (ESP-IDF 5.x, desktop): legacy mbedtls_* API.
//
// Define RECSRMESH_CRYPTO_PSA or RECSRMESH_CRYPTO_LEGACY to force a backend.
// Elliptic-curve work (secp192r1) is not done here: Mbed TLS 4 removed that
// curve, so it is handled by the bundled micro-ecc (see recsrmesh/uecc.h).

#include "recsrmesh/types.h"

#include <cstddef>
#include <cstdint>

namespace csrmesh::crypto::backend {

inline constexpr size_t SHA256_LEN = 32;

// SHA-256 over the concatenation a || b (either may be empty).
Error sha256(ByteSpan a, ByteSpan b, uint8_t out[SHA256_LEN]);

inline Error sha256(ByteSpan a, uint8_t out[SHA256_LEN])
{
    return sha256(a, ByteSpan{}, out);
}

// AES-128 in OFB mode starting at IV offset 0 (encrypt == decrypt).
// iv is used as scratch and is modified.
Error aes128_ofb(const uint8_t key[16], uint8_t iv[16],
                 const uint8_t *in, uint8_t *out, size_t len);

// Cryptographically secure random bytes.
Error random_bytes(uint8_t *out, size_t len);

// "psa" or "legacy" (for logs/tests).
const char *name();

}  // namespace csrmesh::crypto::backend
