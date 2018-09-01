#include <iostream>
#include <YbMesh/YbMesh.hpp>
#include <easylogging++.h>
#include <glm/vec3.hpp>// glm::vec3
#include <glm/geometric.hpp>// glm::cross, glm::normalize
#include <cmath>
using std::shared_ptr;
using std::cout;

int main() {
//    auto mesh = YbMesh::IO::importOBJ_V1("/Users/hotpot/data/mesh_data/body2.obj").second;
//    auto he =  YbMesh::SharedHalfEdge(mesh).toMesh();
//    YbMesh::IO::exportObj(he, "/Users/hotpot/data/mesh_data/body2he.obj");
    auto mesh = YbMesh::IO::importOBJ_V0("/Users/hotpot/data/mesh_data/bunny.obj");
    auto heMesh = YbMesh::SharedHalfEdge(mesh);
    YbMesh::sqrt3_subdivision(heMesh);
//    YbMesh::sqrt3_subdivision(heMesh);
    auto he =  heMesh.toMesh();
    YbMesh::IO::exportObj( he, "/Users/hotpot/data/mesh_data/bunny1.obj");
}
