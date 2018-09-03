//
// Created by user on 2018/8/17.
//

#include <iostream>
#include <vector>
#include "HalfEdge.h"

#define SORT_OPTIMIZATION
namespace YbMesh {

SharedHalfEdge* SharedHalfEdge::ptr = nullptr;

SharedHalfEdge::SharedHalfEdge(IMesh &mesh): edges(mesh.f().size()*3),
        findex(mesh.f().size(), -1), verts(mesh.v().size()){
    ptr = this;
    for(int i = 0; i < mesh.v().size(); i++)
        verts[i].v = mesh.v()[i];
    // 构建基础半边结构
    auto eit = edges.begin();
    for(auto it = mesh.f().begin(); it != mesh.f().end(); it++) {
        findex[it - mesh.f().begin()] = eit - edges.begin();
        // rg边
        verts[it->r].edge = eit - edges.begin();
        eit->_from = it->r; eit->_to = it->g;
        eit->_face = it - mesh.f().begin();
        eit->_next = eit - edges.begin() + 1; eit++;
        // gb边
        verts[it->g].edge = eit - edges.begin();
        eit->_from = it->g; eit->_to = it->b;
        eit->_face = it - mesh.f().begin();
        eit->_next = eit - edges.begin() + 1; eit++;
        // br边
        verts[it->b].edge = eit - edges.begin();
        eit->_from = it->b; eit->_to = it->r;
        eit->_face = it - mesh.f().begin();
        eit->_next = eit - edges.begin() - 2; eit++;
    }

#ifdef SORT_OPTIMIZATION
    std::vector<int> edges_ref(edges.size());
    for(int i = 0; i < edges_ref.size(); i++) edges_ref[i] = i;
    std::sort(edges_ref.begin(), edges_ref.end(), [&](const int& i1, const int& i2){
        auto &e1 = edges[i1];
        auto &e2 = edges[i2];
        return e1._from == e2._from ? e1._to < e2._to: e1._from < e2._from;
    });

    for(auto it = edges_ref.begin(); it != edges_ref.end(); it++) {
        auto ref = std::lower_bound(edges_ref.begin(), edges_ref.end(), edges[*it] ,[=] (const int& _e,const IHalfEdge& v){
            auto &e = edges[_e];
            return e._from == v._to ? e._to < v._from : e._from < v._to;
        });
        if(ref != edges_ref.end() && edges[*it]._from == edges[*ref]._to && edges[*it]._to == edges[*ref]._from)
            edges[*it]._pair = ref != edges_ref.end()?*ref:-1;
    }
#else
    for(eit = edges.begin(); eit != edges.end(); eit++) {
        eit->_pair = std::find_if(edges.begin(), edges.end(),[=]
                (const IHalfEdge& v){
            return v._to == eit->_from && v._from == eit->_to;
        }) - edges.begin();
    }
#endif
    int cnt  = 0;
    for(auto it = edges.begin(); it !=  edges.end(); it++) {
        if(it->pair() == edges.end()) cnt++;
        else if(it->pair()->pair() != it) {
            exit(-1);
        }
    }
    std::cout<<"border face: " << cnt << std::endl;
}


IMesh SharedHalfEdge::toMesh() {
    auto vs = std::make_shared<std::vector<glm::vec3>>();
    auto fs = std::make_shared<std::vector<glm::ivec3>>();
    vs->reserve(verts.size());
    fs->reserve(edges.size()/3);
    for(int i = 0; i < verts.size(); i++) vs->emplace_back(verts[i].v);
    std::vector<bool> edges_flag(edges.size(), false);
    for(int i = 0; i < edges.size(); i++) {
        if(!edges_flag[i]) {
            fs->emplace_back(glm::ivec3(edges[i].to() - verts.begin(), edges[i].next()->to() - verts.begin(),
                                        edges[i].next()->next()->to() - verts.begin() ));
            edges_flag[i] = true;
            edges_flag[edges[i].next()-edges.begin()] = true;
            edges_flag[edges[i].next()->next()-edges.begin()] = true;
        }
    }
    return IMesh(vs,fs);
}

IHalfEdgeIt SharedHalfEdge::edge(int f) {
    return this->edges.begin() + f;
}

IHalfEdgeIt IHalfEdge::next() {
    return SharedHalfEdge::ptr->edges.begin() + _next;
}

IHalfEdgeIt IHalfEdge::pair() {
    return _pair == -1?SharedHalfEdge::ptr->edges.end():(SharedHalfEdge::ptr->edges.begin() + _pair);
}

VertIt IHalfEdge::to() {
    return SharedHalfEdge::ptr->verts.begin()+_to;
}

VertIt IHalfEdge::from() {
    return SharedHalfEdge::ptr->verts.begin()+_from;
}

glm::vec3 IHalfEdge::vector() {
    return to()->v - from()->v;
}

}
