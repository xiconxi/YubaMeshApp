#ifndef TEXTUREFACEMESH_H
#define TEXTUREFACEMESH_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace Plugin {
namespace parameterize {

template <class T>
struct indicesTriMesh
{
    std::vector<T> v;
    std::vector<glm::ivec3> f;
};

std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> loadObj(std::string file);

template <class T>
void border(indicesTriMesh<T>& mesh);

//#ifndef TEXTUREFACEMESH_CPP //// needed by windows ?
////extern template void border(indicesTriMesh<glm::vec2>& mesh);
////extern template void border(indicesTriMesh<glm::vec3>& mesh);
//#endif

}

}


#endif // TEXTUREFACEMESH_H
