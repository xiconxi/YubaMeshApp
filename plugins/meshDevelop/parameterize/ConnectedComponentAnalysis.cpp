#include "ConnectedComponentAnalysis.h"
#include <algorithm>
#include <easylogging++.h>
auto compare = [](const glm::ivec2& e1, const glm::ivec2& e2){
    return e1[0] == e2[0] ? (e1[1] > e2[1]):e1[0] > e2[0];
};
TriMeshComponent::TriMeshComponent(glm::ivec3 face, uint32_t id):border_edge(compare)
{
    border_edge[glm::ivec2(face[2],face[1])] = id;
    border_edge[glm::ivec2(face[1],face[0])] = id;
    border_edge[glm::ivec2(face[0],face[2])] = id;
}

size_t TriMeshComponent::size() {
    return component_faces.size();
}

bool TriMeshComponent::isBorderMatched(glm::ivec3 face) {
    return border_edge.count(glm::ivec2(face[0],face[1])) ||
            border_edge.count(glm::ivec2(face[1],face[2])) ||
            border_edge.count(glm::ivec2(face[2],face[0]));
}

void TriMeshComponent::merge(TriMeshComponent *srcComponent, glm::ivec3 face) {
    // 在srcComponent中移除桥接边
    for(auto& e:{glm::ivec2(face[1],face[0]),glm::ivec2(face[0],face[2]),glm::ivec2(face[2],face[1])}) {
        if(srcComponent->border_edge.count(e)){
            srcComponent->component_faces.insert(border_edge[e]);
            srcComponent->border_edge.erase(border_edge.find(e));
        }
    }
    this->border_edge.insert(srcComponent->border_edge.begin(), srcComponent->border_edge.end());
    this->component_faces.insert(srcComponent->component_faces.begin(),srcComponent->component_faces.end());
}

void TriMeshComponent::merge(glm::ivec3 face, uint32_t id) {
    for(auto& e:{glm::ivec2(face[0],face[1]),glm::ivec2(face[1],face[2]),glm::ivec2(face[2],face[0])}) {
        if(border_edge.count(e) == 0) {
            border_edge[glm::ivec2(e[1],e[0])] = id;
        }else {
            component_faces.insert(border_edge[e]);
            border_edge.erase(border_edge.find(e));
        }
    }
}

std::list<TriMeshComponent*>  TriMeshComponentAnalysis(std::vector<glm::ivec3>& faces) {
    // possible sequential operate before component analysis
    std::list<TriMeshComponent*> components;
    auto it = faces.begin();
    auto match_condiction = [&it](TriMeshComponent* e){ return e->isBorderMatched(*it);};
    for(; it != faces.end(); it++) {
        auto face_owner_it = std::find_if(components.begin(), components.end(), match_condiction);
        if(face_owner_it == components.end())
            components.push_front(new TriMeshComponent(*it,it-faces.begin()));
        else {
            (*face_owner_it)->merge(*it,it-faces.begin());
            auto next_it = std::find_if(std::next(face_owner_it), components.end(), match_condiction);
            if(next_it != components.end()) {
                (*face_owner_it)->merge(*next_it,*it);
                delete *next_it;
                components.erase(next_it++);
            }
            next_it = std::find_if(next_it, components.end(), match_condiction);
            if(next_it != components.end()) {
                (*face_owner_it)->merge(*next_it,*it);
                delete *next_it;
                components.erase(next_it);
            }
        }
        if((it-faces.begin())%(faces.size()/1000) == 0)
            LOG(INFO) << "percent: " << float (it-faces.begin())/(faces.size());
    }
    return components;
}
