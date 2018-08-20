#ifndef HALF_EDGE_H
#define HALF_EDGE_H

#include <map>
#include <stdint.h>
#include "PickableMesh.h"
/*
哪些面使用了这个顶点
哪些边使用了这个顶点
哪些面使用了这条边
哪些边构成了这个面
哪些面和这个面相邻
*/
class halfEdge
{
    struct halfedge_t
    {
        uint face, vert; // vertex at the end of the half-edge

        uint next, pair;
    };
public:
    halfEdge(PickableMesh* mesh,std::vector<glm::ivec3>::iterator f_begin, std::vector<glm::ivec3>::iterator f_end);
    std::vector<int> get_connected_vert(uint vert_id);
    std::vector<std::array<glm::vec3,2>> get_slice(glm::vec3 n, float d, float gap);
private:
    std::map<uint, uint> v_he;  // one of the half-edges emantating from the vertex
    std::vector<halfedge_t> he; // half-edges
    std::map<uint, uint> f_he;  // one of the half-edges bordering the face
    PickableMesh* mesh;
};

#endif // HALF_EDGE_H
