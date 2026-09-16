#include "avionmesh/commands.h"

#include <cstring>

namespace avionmesh {

// target_id < GROUP_THRESHOLD → group broadcast (dest_id=0, group in payload)
// target_id >= GROUP_THRESHOLD → direct device (dest_id=target_id, group=0)
static void create_packet(uint16_t target_id, Verb verb, Noun noun,
                          const uint8_t *value, size_t value_len,
                          Command &cmd)
{
    uint16_t group_id, dest_id;
    if (target_id < GROUP_THRESHOLD) {
        group_id = target_id;
        dest_id = 0;
    } else {
        group_id = 0;
        dest_id = target_id;
    }

    cmd.dest_id = dest_id;
    std::memset(cmd.payload, 0, sizeof(cmd.payload));
    cmd.payload[0] = static_cast<uint8_t>(verb);
    cmd.payload[1] = static_cast<uint8_t>(noun);
    cmd.payload[2] = static_cast<uint8_t>(group_id >> 8);
    cmd.payload[3] = static_cast<uint8_t>(group_id & 0xFF);
    cmd.payload[4] = 0;

    size_t copy = value_len;
    if (copy > 3) copy = 3;
    std::memcpy(cmd.payload + 5, value, copy);

    cmd.payload_len = 10;
}

void cmd_brightness(uint16_t target_id, uint8_t brightness, Command &cmd)
{
    uint8_t value[3] = {brightness, 0, 0};
    create_packet(target_id, Verb::Write, Noun::Dimming, value, 3, cmd);
}

void cmd_color_temp(uint16_t target_id, uint16_t kelvin, Command &cmd)
{
    uint8_t value[3] = {
        0x01,
        static_cast<uint8_t>(kelvin >> 8),
        static_cast<uint8_t>(kelvin & 0xFF),
    };
    create_packet(target_id, Verb::Write, Noun::Color, value, 3, cmd);
}

void cmd_read_all_dimming(Command &cmd)
{
    uint8_t value[3] = {0, 0, 0};
    create_packet(0, Verb::Read, Noun::Dimming, value, 3, cmd);
}

void cmd_read_all_color(Command &cmd)
{
    uint8_t value[3] = {0, 0, 0};
    create_packet(0, Verb::Read, Noun::Color, value, 3, cmd);
}

void cmd_set_date(uint16_t year, uint8_t month, uint8_t day, Command &cmd)
{
    uint8_t value[3] = {static_cast<uint8_t>(year - 2000), month, day};
    create_packet(0, Verb::Write, Noun::Date, value, 3, cmd);
}

void cmd_set_time(uint8_t hour, uint8_t minute, uint8_t second, Command &cmd)
{
    uint8_t value[3] = {hour, minute, second};
    create_packet(0, Verb::Write, Noun::Time, value, 3, cmd);
}

void cmd_ping(uint16_t target_id, Command &cmd)
{
    uint8_t value[3] = {0x01, 0, 0};
    create_packet(target_id, Verb::Ping, Noun::None, value, 3, cmd);
}

void cmd_insert_group(uint16_t device_id, uint16_t group_id, Command &cmd)
{
    uint8_t value[3] = {
        static_cast<uint8_t>(group_id >> 8),
        static_cast<uint8_t>(group_id & 0xFF),
        0,
    };
    create_packet(device_id, Verb::Insert, Noun::Groups, value, 3, cmd);
}

void cmd_delete_group(uint16_t device_id, uint16_t group_id, Command &cmd)
{
    uint8_t value[3] = {
        static_cast<uint8_t>(group_id >> 8),
        static_cast<uint8_t>(group_id & 0xFF),
        0,
    };
    create_packet(device_id, Verb::Delete, Noun::Groups, value, 3, cmd);
}

void cmd_create(uint16_t target_id, Verb verb, Noun noun,
                const uint8_t *value, size_t value_len, Command &cmd)
{
    create_packet(target_id, verb, noun, value, value_len, cmd);
}

}  // namespace avionmesh
