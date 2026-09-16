#include "avionmesh/avionmesh.h"

namespace avionmesh {

csrmesh::Error send_cmd(csrmesh::MeshContext &mesh, const Command &cmd)
{
    return csrmesh::send(mesh, cmd.dest_id, MODEL_OPCODE,
                         cmd.payload, cmd.payload_len);
}

}  // namespace avionmesh
