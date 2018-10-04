#include <iostream>
#include <YbMesh/YbMesh.hpp>
#include <easylogging++.h>
#include <glm/vec3.hpp>// glm::vec3
#include <glm/geometric.hpp>// glm::cross, glm::normalize
#include <cmath>
#include <stack>
#include <iterator>
#include <Eigen/Sparse>

using std::shared_ptr;
using std::cout;
#define PI 3.1415926f

namespace paramarization {
    std::vector<int> borderExtraction(YbMesh::SharedHalfEdge& heMesh){
         std::vector<int> m;
         // Half Edge Implement
         auto null_it = heMesh.edges.end();
         auto s_edge = std::find_if(heMesh.edges.begin(), heMesh.edges.end(),
                                    [=](YbMesh::IHalfEdge& it){ return it._pair == -1; });
         auto curr = s_edge;
         do{
             m.emplace_back(curr->_to);
             while(curr->next()->pair() != null_it)
                 curr = curr->next()->pair();
             curr = curr->next();
         }while(s_edge != curr);
         return m;
    }

    std::pair<std::vector<float>,std::vector<float>> make_circle(int verts){
        std::vector<float> u,v;
        u.reserve(verts); v.reserve(verts);
        for(float alpha = 2*PI; alpha > 0; alpha -= 2*PI/verts ){
            u.emplace_back(std::sinf(alpha));
            v.emplace_back(std::cosf(alpha));
        }

        return std::make_pair(u,v);
    }

    void test(YbMesh::indicesTriMesh<glm::vec3>& mesh) {
        auto heMesh = YbMesh::SharedHalfEdge(mesh);
        auto n = mesh.v().size();
        auto m = heMesh.rerange(paramarization::borderExtraction(heMesh));
        m = heMesh.rerange(paramarization::borderExtraction(heMesh));
        auto circle = paramarization::make_circle(m);


        // Laplace Matrix
        std::vector<float> w_ii(n,0.0f);
        std::vector<Eigen::Triplet<float>> border_triplet;
        std::vector<Eigen::Triplet<float>> inner_triplet;
        for(auto& edge:heMesh.edges) {
            if(edge._from < m) continue;
            if(edge._to < m){ // L(border,inner)
                border_triplet.emplace_back(Eigen::Triplet<float>{ edge._from-m, edge._to, 1.0f});
            }else if(edge._to >= m) { // L(inner,inner)
                inner_triplet.emplace_back(Eigen::Triplet<float>{ edge._from-m, edge._to-m, 1.0f});
            }
            w_ii[edge._from] -= 1.0f;
        }
        for(int i = m; i < n; i++)
            inner_triplet.emplace_back(Eigen::Triplet<float>{ i-m,i-m,w_ii[i]});

        Eigen::SparseMatrix<float> inner(n-m, n-m);
        Eigen::SparseMatrix<float> border(n-m, m);
        inner.setFromTriplets(inner_triplet.begin(), inner_triplet.end());
        border.setFromTriplets(border_triplet.begin(), border_triplet.end());

        // linear system
        Eigen::VectorXf c_u(m), p_u(n-m), c_v(m), p_v(n-m);
        for(int i = 0; i < m; i++) {
            c_u[i] = -circle.first[i];
            c_v[i] = -circle.second[i];
        }
        Eigen::SparseLU<Eigen::SparseMatrix<float>> solver;
        solver.analyzePattern(inner);
        solver.factorize(inner);
        p_u = solver.solve(border*c_u);
        p_v = solver.solve(border*c_v);

        auto tex = std::make_shared<std::vector<glm::vec2>>(n);
        for(int i = 0; i< m; i++)
            (*tex)[i] = glm::vec2(c_u[i],c_v[i]);
        for(int i = m; i < n; i++)
            (*tex)[i] = glm::vec2(p_u[i-m],p_v[i-m]);
//        auto new_f = std::make_shared<std::vector<glm::ivec3>>();
//        new_f->reserve(mesh.f().size());
//        for(auto& e:mesh.f()) {
//            if(e.r > m && e.g > m && e.b > m)
//                new_f->emplace_back(e);
//        }
        auto msh = heMesh.toMesh();
        YbMesh::IO::exportObj("/Users/hotpot/data/mesh_data/paramarization_tex1.obj",msh.shared_f(), msh.shared_v(), tex);
//        YbMesh::IO::exportObj("/Users/hotpot/data/mesh_data/paramarization_tex.obj",mesh.shared_f(), mesh.shared_v(), tex);
//        YbMesh::IO::exportObj("/Users/hotpot/data/mesh_data/paramarization_tex.obj",new_f, mesh.shared_v(), tex);
    }

}


int main() {
//    auto mesh = YbMesh::IO::importOBJ_V1("/Users/hotpot/data/mesh_data/body2.obj").second;
//    auto he =  YbMesh::SharedHalfEdge(mesh).toMesh();
//    YbMesh::IO::exportObj(he, "/Users/hotpot/data/mesh_data/body2he.obj");
    auto mesh = YbMesh::IO::importOBJ_V0("/Users/hotpot/data/mesh_data/body2_head2.obj"); // head_ranged
    paramarization::test(mesh);
//    YbMesh::sqrt3_subdivision(heMesh);
////    YbMesh::sqrt3_subdivision(heMesh);
//    auto he =  heMesh.toMesh();
//    YbMesh::IO::exportObj( he, "/Users/hotpot/data/mesh_data/bunny1.obj");
}
