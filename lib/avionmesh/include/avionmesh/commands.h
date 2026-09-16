#pragma once

#include "types.h"

namespace avionmesh {

enum class Verb : uint8_t {
    Write    = 0,
    Read     = 1,
    Insert   = 2,
    Truncate = 3,
    Count    = 4,
    Delete   = 5,
    Ping     = 6,
    Sync     = 7,
};

enum class Noun : uint8_t {
    Groups          = 3,
    Schedule        = 7,
    Countdown       = 9,
    Dimming         = 10,
    Date            = 21,
    Time            = 22,
    FadeTime        = 25,
    Association     = 27,
    Color           = 29,
    Config          = 30,
    FirmwareVersion = 40,
    None            = 255,
};

struct Command {
    uint16_t dest_id = 0;
    uint8_t  payload[10] = {};
    size_t   payload_len = 0;
};

void cmd_brightness(uint16_t target_id, uint8_t brightness, Command &cmd);
void cmd_color_temp(uint16_t target_id, uint16_t kelvin, Command &cmd);
void cmd_read_all_dimming(Command &cmd);
void cmd_read_all_color(Command &cmd);
void cmd_set_date(uint16_t year, uint8_t month, uint8_t day, Command &cmd);
void cmd_set_time(uint8_t hour, uint8_t minute, uint8_t second, Command &cmd);
void cmd_ping(uint16_t target_id, Command &cmd);
void cmd_insert_group(uint16_t device_id, uint16_t group_id, Command &cmd);
void cmd_delete_group(uint16_t device_id, uint16_t group_id, Command &cmd);

void cmd_create(uint16_t target_id, Verb verb, Noun noun,
                const uint8_t *value, size_t value_len, Command &cmd);

}  // namespace avionmesh
