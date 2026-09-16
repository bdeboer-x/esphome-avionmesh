#pragma once
// Bundled micro-ecc (BSD-2-Clause, external/micro-ecc), configured for the
// only curve CSRMesh MASP uses: secp192r1. Mbed TLS 4 / ESP-IDF 6 removed
// that curve, so it cannot come from the platform crypto library.
//
// All public symbols are prefixed (recsrmesh_uECC_*) so this copy can never
// clash with another micro-ecc linked into the same firmware.

// Portable C implementation on every target (ESP32 already uses it); the
// bundled copy omits micro-ecc's ARM/AVR assembly files.
#define uECC_PLATFORM 0 /* uECC_arch_other */

#define uECC_SUPPORTS_secp160r1 0
#define uECC_SUPPORTS_secp192r1 1
#define uECC_SUPPORTS_secp224r1 0
#define uECC_SUPPORTS_secp256r1 0
#define uECC_SUPPORTS_secp256k1 0
#define uECC_SUPPORT_COMPRESSED_POINT 0
#define uECC_ENABLE_VLI_API 0

#define uECC_secp160r1              recsrmesh_uECC_secp160r1
#define uECC_secp192r1              recsrmesh_uECC_secp192r1
#define uECC_secp224r1              recsrmesh_uECC_secp224r1
#define uECC_secp256r1              recsrmesh_uECC_secp256r1
#define uECC_secp256k1              recsrmesh_uECC_secp256k1
#define uECC_set_rng                recsrmesh_uECC_set_rng
#define uECC_get_rng                recsrmesh_uECC_get_rng
#define uECC_curve_private_key_size recsrmesh_uECC_curve_private_key_size
#define uECC_curve_public_key_size  recsrmesh_uECC_curve_public_key_size
#define uECC_make_key               recsrmesh_uECC_make_key
#define uECC_shared_secret          recsrmesh_uECC_shared_secret
#define uECC_compress               recsrmesh_uECC_compress
#define uECC_decompress             recsrmesh_uECC_decompress
#define uECC_valid_public_key       recsrmesh_uECC_valid_public_key
#define uECC_compute_public_key     recsrmesh_uECC_compute_public_key
#define uECC_sign                   recsrmesh_uECC_sign
#define uECC_sign_deterministic     recsrmesh_uECC_sign_deterministic
#define uECC_verify                 recsrmesh_uECC_verify
#define uECC_sign_with_k            recsrmesh_uECC_sign_with_k

#include "../../../micro-ecc/uECC.h"
