#ifndef VISUALIZATION_H
#define VISUALIZATION_H
#include "./types/type_mesh.h"
#include <glm/matrix.hpp>
namespace YbMesh {
namespace visualization {
    void LIBSHARED_EXPORT calculateNorm(indicesTriMesh<glm::vec3>& vMesh, indicesTriMesh<glm::vec3>& nMesh);
    glm::mat4 LIBSHARED_EXPORT normalize(indicesTriMesh<glm::vec3>& vMesh, bool centerlized = true);
    glm::mat4 LIBSHARED_EXPORT boundingBox(indicesTriMesh<glm::vec3>& vMesh, glm::mat3 crd);
}
}


#endif // VISUALIZATION_H
