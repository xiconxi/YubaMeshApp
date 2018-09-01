#include "sqrt3.h"
#include "../types/HalfEdge.h"
#include <easylogging++.h>
#include <glm/gtx/transform.hpp>

void YbMesh::sqrt3_subdivision(YbMesh::SharedHalfEdge& heMesh) {
    using YbMesh::IHalfEdgeIt;

    auto& edges = heMesh.edges;
    auto& verts = heMesh.verts;
    auto& faces = heMesh.findex;

    //// step 1: 添加新顶点
    {
        edges.reserve(edges.size() *3);
        verts.reserve(verts.size() + heMesh.findex.size());
        int origin_face = faces.size();
        faces.resize(3 * origin_face);
        for(int i = 0; i < origin_face; i++) {
            YbMesh::IHalfEdgeIt ab = heMesh.edge(heMesh.findex[i]);
            YbMesh::IHalfEdgeIt bc = ab->next(), ca = ab->next()->next();
            YbMesh::VertIt a = ca->to(), b = ab->to(), c = bc->to();
            verts.emplace_back(YbMesh::Vert{(int)edges.size(), (a->v + b->v + c->v)/3.0f});
            auto p = std::prev(verts.end());

            edges.emplace_back(YbMesh::IHalfEdge(p-verts.begin(), a-verts.begin()));
            edges.emplace_back(YbMesh::IHalfEdge(p-verts.begin(), b-verts.begin()));
            edges.emplace_back(YbMesh::IHalfEdge(p-verts.begin(), c-verts.begin()));
            edges.emplace_back(YbMesh::IHalfEdge(a-verts.begin(), p-verts.begin()));
            edges.emplace_back(YbMesh::IHalfEdge(b-verts.begin(), p-verts.begin()));
            edges.emplace_back(YbMesh::IHalfEdge(c-verts.begin(), p-verts.begin()));
            IHalfEdgeIt ap = edges.end() - 3, bp = edges.end() - 2, cp = edges.end() - 1;
            IHalfEdgeIt pa = edges.end() - 6, pb = edges.end() - 5, pc = edges.end() - 4;
            // pair
            pa->_pair = ap - edges.begin(); ap->_pair = pa - edges.begin();
            pb->_pair = bp - edges.begin(); bp->_pair = pb - edges.begin();
            pc->_pair = cp - edges.begin(); cp->_pair = pc - edges.begin();

            // triangle(a,b,p)
            faces[0*origin_face + i] = ab - edges.begin();
            ab->_next = bp - edges.begin(); bp->_next = pa - edges.begin(); pa->_next = ab - edges.begin();
//            ab->_prev = pa - edges.begin(); bp->_prev = ab - edges.begin(); pa->_prev = bp - edges.begin();
            // triangle(b,c,p)
            faces[1*origin_face + i] = bc - edges.begin();
            bc->_next = cp - edges.begin(); cp->_next = pb - edges.begin(); pb->_next = bc - edges.begin();
//            bc->_prev = pb - edges.begin(); cp->_prev = bc - edges.begin(); pb->_prev = cp - edges.begin();
            // triangle(c,a,p)
            faces[2*origin_face + i] = ca - edges.begin();
            ca->_next = ap - edges.begin(); ap->_next = pc - edges.begin(); pc->_next = ca - edges.begin();
//            ca->_prev = pa - edges.begin(); ap->_prev = ca - edges.begin(); pc->_prev = ap - edges.begin();
        }

    }


    //// step 2: 翻转网格
    {
        std::vector<bool> visited_flag(edges.size()/3,false);
        for(auto it = edges.begin(); it != edges.begin() + edges.size()/3; it++) {
            // 一条边只能翻转局部网格一次
            if(visited_flag[it - edges.begin()] || it->pair() == edges.end() || visited_flag[it->pair() - edges.begin()]) continue;
            visited_flag[it - edges.begin()] = visited_flag[it->pair() - edges.begin()] = true;
            //    T(C,B,A)+T(D,A,B)     ===>      T(A,C,D)+T(B,D,C)
            // (CB, BA, AC|DA, AB, BD)  ===>   (AC, CD, DA|BD, DC, CB)
            //                      AB  ===>   CD
            auto ba = it, ab = it->pair();
            auto ac = ba->next(), bd = ab->next();
            auto cb = ac->next(), da = bd->next();

            // 筛除无法翻转的情况( sin(<AC,AD>) < 0 || sin(<BC,BD>) < 0 )
            float dotv = glm::dot(glm::normalize(glm::cross(ba->to()->v - ac->to()->v,ba->to()->v - bd->to()->v)),
                                  glm::normalize(glm::cross(ab->to()->v - ac->to()->v,ab->to()->v - bd->to()->v)) );
            if( dotv > 0)
                continue;

            auto cd = ab, dc = ba;
            cd->_to = bd->_to; dc->_to = ac->_to;
            da->_next = ac - edges.begin(); ac->_next = cd - edges.begin(); cd->_next = da - edges.begin();
            cb->_next = bd - edges.begin(); bd->_next = dc - edges.begin(); dc->_next = cb - edges.begin();
        }
    }

    // step 3: 更新所有顶点坐标值
    std::vector<glm::vec3> vs1(verts.size());
    for(int i = 0; i < vs1.size(); i++) {
        std::pair<glm::vec3, int> v{glm::vec3(0.0f),0};
        auto it = heMesh.edge(verts[i].edge), curr = heMesh.edge(verts[i].edge);
        do{
            v.first += curr->to()->v;
            v.second++;
            if(curr->pair() == edges.end()) break;
            curr = curr->pair()->next();
            if(v.second > 30) exit(-1);
        }while(it != curr);
        vs1[i] = verts[i].v*0.4f+v.first*(0.6f/v.second); //
    }
    for(int i = 0; i < vs1.size(); i++)  verts[i].v = vs1[i];
}
