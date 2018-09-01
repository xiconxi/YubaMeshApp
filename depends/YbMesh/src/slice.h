#ifndef SLICE_H
#define SLICE_H
#include "types/type_mesh.h"
#include <glm/matrix.hpp>
namespace YbMesh {
namespace slice {
    void sortByVector(indicesTriMesh<glm::vec3>& mesh, glm::vec3 x);
    glm::mat3 pca_analysic(indicesTriMesh<glm::vec3> &mesh, uint r_id);
    bool  isFaceInersected(indicesTriMesh<glm::vec3> &mesh,glm::ivec3 f, glm::vec3 n, float d);

    std::array<glm::vec3,2> getFaceIntersection(indicesTriMesh<glm::vec3> &mesh,glm::ivec3 f, glm::vec3 n, float d);

    std::array<std::vector<glm::ivec3>::iterator,2> getSliceInterval(indicesTriMesh<glm::vec3> &mesh,glm::vec3 n, float d, float gap);
}
}

#endif // SLICE_H
