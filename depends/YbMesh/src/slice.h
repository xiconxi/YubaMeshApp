#ifndef SLICE_H
#define SLICE_H
#include "types/type_mesh.h"
#include <glm/matrix.hpp>
namespace YbMesh {
namespace slice {
    void LIBSHARED_EXPORT sortByVector(indicesTriMesh<glm::vec3>& mesh, glm::vec3 x);
    glm::mat3 LIBSHARED_EXPORT pca_analysic(std::vector<glm::vec3>& vs, std::vector<glm::ivec3>::iterator begin,
                                          std::vector<glm::ivec3>::iterator end);
    bool LIBSHARED_EXPORT isFaceInersected(indicesTriMesh<glm::vec3> &mesh,glm::ivec3 f, glm::vec3 n, float d);

    std::array<glm::vec3,2> LIBSHARED_EXPORT getFaceIntersection(indicesTriMesh<glm::vec3> &mesh,glm::ivec3 f, glm::vec3 n, float d);

    std::array<std::vector<glm::ivec3>::iterator,2> LIBSHARED_EXPORT getSliceInterval(indicesTriMesh<glm::vec3> &mesh,glm::vec3 n, float d, float gap);

    typedef std::vector<std::array<glm::vec3,2>> sliceContour;
    typedef std::vector<std::array< sliceContour::iterator,2>> sliceContourInterval;

    namespace _RFF {
        sliceContourInterval LIBSHARED_EXPORT sortContours(sliceContour&& contours);
    }

    template<typename _V ,bool _RV=std::is_rvalue_reference<_V&&>::value,
      typename _ENABLE=typename std::enable_if<std::is_base_of<sliceContour,typename std::decay<_V>::type>::value>::type>
    sliceContourInterval sortContours(_V&& intersections) {
        return _RFF::sortContours(_RV?std::move(intersections):intersections);
    }

}
}

#endif // SLICE_H
