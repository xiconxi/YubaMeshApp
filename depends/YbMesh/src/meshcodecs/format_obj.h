#ifndef FORMAT_OBJ_H
#define FORMAT_OBJ_H
#include "../types/type_mesh.h"
#include <string>
namespace YbMesh {
namespace IO {
    indicesTriMesh<glm::vec3> LIBSHARED_EXPORT importOBJ_V0(std::string file);
    std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> LIBSHARED_EXPORT importOBJ_V1(std::string file);
    indicesTriMesh<glm::vec3> LIBSHARED_EXPORT importOBJ_V2(std::string filename, std::shared_ptr<std::vector<glm::vec2>> vt);

    void LIBSHARED_EXPORT exportObj(indicesTriMesh<glm::vec3>& mesh, std::string file);

    void LIBSHARED_EXPORT exportObj(indicesTriMesh<glm::vec2>& mesh, std::string file);

    void LIBSHARED_EXPORT exportObj(std::string file, std::shared_ptr<std::vector<glm::ivec3>> f,
                                    std::shared_ptr<std::vector<glm::vec3>> v,
                                    std::shared_ptr<std::vector<glm::vec2>> vt = nullptr);

    void LIBSHARED_EXPORT exportObj(std::string file, std::shared_ptr<std::vector<glm::ivec3>> f,
                                    std::shared_ptr<std::vector<glm::vec2>> vt);

    void LIBSHARED_EXPORT writePartialMesh(indicesTriMesh<glm::vec3>& mesh,const std::vector<glm::ivec3> &faces, std::string file_name);
}
}

#endif // FORMAT_OBJ_H
