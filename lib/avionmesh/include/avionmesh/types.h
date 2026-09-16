#pragma once

#include <cstddef>
#include <cstdint>

namespace avionmesh {

inline constexpr uint16_t GROUP_THRESHOLD = 32896;

enum class Product : uint8_t {
    Group        = 0,
    LampDimmer   = 90,
    RecessedDL   = 93,
    LightAdapter = 94,
    SmartDimmer  = 97,
    SmartBulb    = 134,
    SurfaceDL    = 137,
    MicroEdge    = 162,
    SmartSwitch  = 167,
};

bool has_dimming(uint8_t product_type);
bool has_color_temp(uint8_t product_type);
const char *product_name(uint8_t product_type);

struct Status {
    uint16_t avid = 0;
    bool     has_brightness = false;
    uint8_t  brightness = 0;
    bool     has_color_temp = false;
    uint16_t color_temp = 0;
};

}  // namespace avionmesh
