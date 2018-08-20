#ifndef MESHCODES_H
#define MESHCODES_H

#include "../bases/glviewerGlobal.inc"
#include "PickableMesh.h"
#include <string>
#include <stdint.h>

//struct glFormatMesh:public PickableMesh
//{
//    uint tex_buffer;
//    std::vector<std::array<glm::vec3,4>> vts;
//};

namespace YbCore {
namespace IO {
LIBSHARED_EXPORT PickableMesh* readObj(std::string filename);
LIBSHARED_EXPORT PickableMesh* readOff(std::string filename);
LIBSHARED_EXPORT PickableMesh* readCIE(std::string filename);
}
}

#endif // MESHOBJECT_H
