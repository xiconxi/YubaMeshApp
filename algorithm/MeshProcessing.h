#ifndef MESHPROCESSING_H
#define MESHPROCESSING_H

#include <vector>
#include <array>
#include <glm/vec3.hpp>
#include <YbCore/mesh_proc>
#include <type_traits>

namespace plugin{
    typedef std::vector<std::array<glm::vec3,2>> sliceContour;
    typedef std::vector<std::array< sliceContour::iterator,2>> sliceContourInterval;

    namespace _RFF {
        // 以边或面元为单位仅使用源网格顶点导出网格
        void writePartialMesh(PickableMesh* mesh, std::vector<glm::ivec3>&& faces, std::string file_name);
        void writePartialMesh(PickableMesh *mesh, std::vector<glm::ivec2>&& line, std::string file_name);
        void writePartialMesh(PickableMesh *mesh, std::vector<glm::ivec2>&& line, std::fstream fileHandle);

        void writeSlices(PickableMesh* mesh, sliceContour&& slices, std::string file_name);

    }

    template<typename _V ,bool _RV=std::is_rvalue_reference<_V&&>::value>
    void writePartialMesh(PickableMesh* mesh, _V&& faces, std::string file_name) {
        return _RFF::writePartialMesh(mesh, _RV?std::move(faces):faces, file_name);
    }

    template<typename _V ,bool _RV=std::is_rvalue_reference<_V&&>::value,
      typename _ENABLE=typename std::enable_if<std::is_base_of<sliceContour,typename std::decay<_V>::type>::value>::type>
    void writeSlices(PickableMesh* mesh, _V&& slices, std::string file_name) {
        return _RFF::writeSlices(mesh, _RV?std::move(slices):slices, file_name);
    }



    namespace _RFF {
        sliceContourInterval sortContours(sliceContour&& contours);
        std::vector<glm::ivec2> extractMeshBorder(PickableMesh *mesh, std::vector<glm::ivec3>&& faces);
    }

    template<typename _V ,bool _RV=std::is_rvalue_reference<_V&&>::value,
      typename _ENABLE=typename std::enable_if<std::is_base_of<sliceContour,typename std::decay<_V>::type>::value>::type>
    sliceContourInterval sortContours(_V&& intersections) {
        return _RFF::sortContours(_RV?std::move(intersections):intersections);
    }

    template<typename _V ,bool _RV=std::is_rvalue_reference<_V&&>::value,
      typename _ENABLE=typename std::enable_if<std::is_base_of<std::vector<glm::ivec3>,typename std::decay<_V>::type>::value>::type>
    std::vector<glm::ivec2> extractMeshBorder(PickableMesh *mesh,_V&& faces) {
        return _RFF::extractMeshBorder(mesh,_RV?std::move(faces):faces);
    }

}

#endif // MESHPROCESSING_H
