#ifndef FORMAT_OBJ_H
#define FORMAT_OBJ_H
#include "../type_mesh.h"
#include <string>
namespace YbMesh {
namespace IO {
    indicesTriMesh<glm::vec3> importOBJ_V0(std::string file);
    std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> importOBJ_V1(std::string file);

    void exportObj(indicesTriMesh<glm::vec3>& mesh, std::string file);

    void exportObj(indicesTriMesh<glm::vec2>& mesh, std::string file);
}
}

#endif // FORMAT_OBJ_H
