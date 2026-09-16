#include "avionmesh/types.h"

namespace avionmesh {

bool has_dimming(uint8_t product_type)
{
    switch (product_type) {
    case static_cast<uint8_t>(Product::Group):
    case static_cast<uint8_t>(Product::LampDimmer):
    case static_cast<uint8_t>(Product::RecessedDL):
    case static_cast<uint8_t>(Product::LightAdapter):
    case static_cast<uint8_t>(Product::SmartDimmer):
    case static_cast<uint8_t>(Product::SmartBulb):
    case static_cast<uint8_t>(Product::SurfaceDL):
    case static_cast<uint8_t>(Product::MicroEdge):
        return true;
    default:
        return false;
    }
}

bool has_color_temp(uint8_t product_type)
{
    switch (product_type) {
    case static_cast<uint8_t>(Product::Group):
    case static_cast<uint8_t>(Product::RecessedDL):
    case static_cast<uint8_t>(Product::SmartBulb):
    case static_cast<uint8_t>(Product::SurfaceDL):
    case static_cast<uint8_t>(Product::MicroEdge):
        return true;
    default:
        return false;
    }
}

const char *product_name(uint8_t product_type)
{
    switch (product_type) {
    case static_cast<uint8_t>(Product::Group):        return "Group";
    case static_cast<uint8_t>(Product::LampDimmer):   return "Lamp Dimmer";
    case static_cast<uint8_t>(Product::RecessedDL):   return "Recessed Downlight";
    case static_cast<uint8_t>(Product::LightAdapter): return "Light Adapter";
    case static_cast<uint8_t>(Product::SmartDimmer):  return "Smart Dimmer";
    case static_cast<uint8_t>(Product::SmartBulb):    return "Smart Bulb";
    case static_cast<uint8_t>(Product::SurfaceDL):    return "Surface Downlight";
    case static_cast<uint8_t>(Product::MicroEdge):    return "MicroEdge";
    case static_cast<uint8_t>(Product::SmartSwitch):  return "Smart Switch";
    default:                                          return "Unknown";
    }
}

}  // namespace avionmesh
