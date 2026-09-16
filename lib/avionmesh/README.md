# Vendored Avi-on mesh library

Bundled here so the ESPHome component builds with ESPHome's native ESP-IDF
toolchain (no PlatformIO) and on ESP-IDF 6 / Mbed TLS 4.

| Path | Upstream | Commit | License |
| --- | --- | --- | --- |
| `include/`, `src/` | https://github.com/oyvindkinsey/avionmesh-cpp | `bd349f2` | LGPL-3.0-or-later |
| `external/recsrmesh/` | https://github.com/oyvindkinsey/recsrmesh-cpp | `7b71ee1` | LGPL-3.0-or-later |
| `external/micro-ecc/` | https://github.com/kmackay/micro-ecc | `541b3a7` | BSD-2-Clause (`LICENSE.txt`) |

## Local changes to recsrmesh

Mbed TLS 4 (ESP-IDF 6) removed the legacy `mbedtls_*` crypto API and the
secp192r1 curve that CSRMesh MASP pairing requires. To build on both
generations:

- `src/crypto_backend.{h,cpp}` (new): SHA-256, AES-128-OFB and RNG through the
  PSA Crypto API on Mbed TLS 4 (auto-detected via `tf-psa-crypto/build_info.h`),
  or the legacy API on Mbed TLS 2/3. Force with `RECSRMESH_CRYPTO_PSA` or
  `RECSRMESH_CRYPTO_LEGACY`.
- `crypto.cpp`: HMAC-SHA256 implemented per RFC 2104 on top of the backend.
- `mcp_crypto.cpp`: AES-OFB, key derivation and random sequence via the backend.
- `masp.cpp`: secp192r1 ECDH via micro-ecc instead of `mbedtls_ecp_*`.
- `include/recsrmesh/uecc.h`, `src/uecc_p192.c` (new): micro-ecc limited to
  secp192r1, with all symbols prefixed `recsrmesh_uECC_`, forced to the
  portable C implementation. micro-ecc's ARM/AVR assembly files (`asm_*.inc`)
  are not included.

These changes were checked on a host against the unmodified upstream code
(Mbed TLS 3.6): key derivation, HMAC, packet MACs, MCP encrypt/decrypt and
UUID hashes are byte-identical, and ECDH shared secrets match between the old
`mbedtls_ecp` code and micro-ecc. Output on Mbed TLS 4.0 (PSA) is identical to
Mbed TLS 3.6.
