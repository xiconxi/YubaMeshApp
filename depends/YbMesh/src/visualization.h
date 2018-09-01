#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include "./types/type_mesh.h"
#include <glm/matrix.hpp>
namespace YbMesh {
namespace visualization {
    void calculateNorm(indicesTriMesh<glm::vec3>& vMesh, indicesTriMesh<glm::vec3>& nMesh);
    glm::mat4 centerlized(indicesTriMesh<glm::vec3>& vMesh);
}
}


#endif // VISUALIZATION_H
