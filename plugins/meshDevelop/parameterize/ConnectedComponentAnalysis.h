#ifndef CONNECTEDCOMPONENTANALYSIS_H
#define CONNECTEDCOMPONENTANALYSIS_H

#include <vector>
#include <map>
#include <set>
#include <list>
#include <stdint.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <functional>

class TriMeshComponent
{
public:
    TriMeshComponent(glm::ivec3 intial_face, uint32_t id);

    bool isBorderMatched(glm::ivec3 face);

    size_t size();

    void merge(TriMeshComponent* srcComponent,glm::ivec3 face);

    void merge(glm::ivec3 face, uint32_t id);

private:
    // 当前连通分量边界，有向边，无序集合
    typedef std::map<glm::ivec2,uint32_t,std::function<bool(const glm::ivec2&,const glm::ivec2&)> > Map;
    Map border_edge;

    // 当前连通分量内部三角面片(三角形属于当前连通分量，但边都不在border_edge上)
    std::set<uint32_t> component_faces;
};


std::list<TriMeshComponent*>  TriMeshComponentAnalysis(std::vector<glm::ivec3>& faces) ;

#endif // CONNECTEDCOMPONENTANALYSIS_H
