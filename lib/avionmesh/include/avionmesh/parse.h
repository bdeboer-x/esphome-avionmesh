#pragma once

#include "types.h"

namespace avionmesh {

// Parse a decrypted MCP response into avionmesh status.
// mcp_source:    source field from MCP parse (device ID for unicast)
// crypto_source: source field from MCP crypto layer (device ID for broadcast)
// opcode:        MCP opcode (only MODEL_OPCODE=0x73 is processed)
bool parse_response(uint16_t mcp_source, uint16_t crypto_source,
                    uint8_t opcode, const uint8_t *payload,
                    size_t payload_len, Status &status);

}  // namespace avionmesh
