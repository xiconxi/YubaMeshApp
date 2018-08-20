#include "half_edge.h"
#include <easylogging++.h>
#include "meshProcessing.h"

using namespace YbCore;

halfEdge::halfEdge(PickableMesh *mesh, std::vector<glm::ivec3>::iterator f_begin, std::vector<glm::ivec3>::iterator f_end) {
    auto _map = PairKeyMap<int,int>([](const PairKey<int>& e1, const PairKey<int>& e2){
         return (e1.first==e2.first)?(e1.second>e2.second):(e1.first>e2.first);
    });
    he.reserve((f_end-f_begin)*3);
    for (auto it = f_begin; it != f_end; it++) {
        for (int i:{0,1,2}) {
            he.push_back(halfedge_t{uint(it-mesh->f.begin()), uint((*it)[i==2?0:i+1]), uint(he.size() + (i==2?-2:1)),0});
            if(_map.count(std::make_pair((*it)[i],(*it)[(i+1)%3])) == 0){
                _map[std::make_pair((*it)[(i+1)%3],(*it)[i])] = he.size() - 1;
            } else {
                he[_map[std::make_pair((*it)[i], (*it)[(i+1)%3])]].pair = he.size() - 1;
                he[he.size() - 1].pair = _map[std::make_pair((*it)[i], (*it)[(i+1)%3])];
            }
            if(v_he.count((*it)[i]) == 0)
                v_he[(*it)[i]] = he.size() - 1;
        }
        f_he[uint(it-mesh->f.begin())] = he.size() - 3;
    }
    this->mesh = mesh;
}


std::vector<int> halfEdge::get_connected_vert(uint vert_id) {
    std::vector<int> connected_vert;
    auto start_id = v_he[vert_id], curr_id = v_he[vert_id];
    do{
        connected_vert.push_back(he[curr_id].vert);
        curr_id = he[he[curr_id].pair].next;
    }while(start_id != curr_id);
    return connected_vert;
}

std::vector<std::array<glm::vec3,2>> halfEdge::get_slice(glm::vec3 n, float d, float gap) {
    std::vector<std::array<glm::vec3,2>> result;
    auto p1 = std::lower_bound(mesh->f.begin(),mesh->f.end(), d-gap,
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]], n);
    });

    auto p2 = std::lower_bound(mesh->f.begin(),mesh->f.end(), d+gap,
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]], n);
    });

    for(auto it = p1; it != p2; it++) {
        if(isFaceInersected(mesh, *it, n, d)) {
            result.emplace_back(getFaceIntersection(mesh, *it, n, d));
        }
    }

    LOG(INFO) << "slice: " << p2 - p1 << ' ' << result.size();
    return result;
}

