#pragma once

#include <recsrmesh/csrmesh.h>

#include "commands.h"
#include "parse.h"
#include "types.h"

namespace avionmesh {

inline constexpr uint8_t MODEL_OPCODE = 0x73;

csrmesh::Error send_cmd(csrmesh::MeshContext &mesh, const Command &cmd);

}  // namespace avionmesh
