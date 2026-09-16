#include "avionmesh/parse.h"
#include "avionmesh/commands.h"

#include <cstring>

namespace avionmesh {

static constexpr uint8_t MODEL_OPCODE = 0x73;

bool parse_response(uint16_t mcp_source, uint16_t crypto_source,
                    uint8_t opcode, const uint8_t *payload,
                    size_t payload_len, Status &status)
{
    if (opcode != MODEL_OPCODE)
        return false;
    if (payload_len < 2)
        return false;
    if (payload[0] == 0 && payload[1] == 0)
        return false;

    auto verb = static_cast<Verb>(payload[0]);
    auto noun = static_cast<Noun>(payload[1]);

    // Broadcast (source=0x8000) uses crypto_source for device ID
    uint16_t device_id = (mcp_source == 0x8000) ? crypto_source : mcp_source;

    status = {};
    status.avid = device_id;

    const uint8_t *value_bytes;
    if (verb == Verb::Write) {
        // WRITE responses: skip 2-byte group field after verb+noun
        if (payload_len < 4)
            return false;
        value_bytes = payload + 4;
        payload_len -= 4;
    } else {
        value_bytes = payload + 2;
        payload_len -= 2;
    }

    if (noun == Noun::Dimming) {
        if (payload_len < 2)
            return false;
        status.has_brightness = true;
        status.brightness = value_bytes[1];
        return true;
    } else if (noun == Noun::Color) {
        if (payload_len < 4)
            return false;
        status.has_color_temp = true;
        status.color_temp = (static_cast<uint16_t>(value_bytes[2]) << 8) | value_bytes[3];
        return true;
    }

    return false;
}

}  // namespace avionmesh
