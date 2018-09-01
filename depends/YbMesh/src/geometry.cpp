#include "geometry.h"
#include <cmath>
#include <iostream>

YbMesh::indicesTriMesh<glm::vec3> YbMesh::geometry::make_spherical(int rows) {
    const float pi = 3.1415926f;
    using namespace std;
    auto vs = make_shared<vector<glm::vec3>>();
    auto fs = make_shared<vector<glm::ivec3>>();
    vs->emplace_back(glm::vec3(.0f,.0f, 1.0f));
    for(float j = pi; j >= 0.0f; j -= pi/rows){
        for(float i = 2*pi; i >= 0.0f; i -= 2*pi/rows) {
            vs->emplace_back(glm::vec3(sinf(j)*cosf(i), sinf(j)*sinf(i),cosf(j)));
        }
    }
    return YbMesh::indicesTriMesh<glm::vec3>(vs,fs);
}

YbMesh::indicesTriMesh<glm::vec3> YbMesh::geometry::make_cylinders(float radius,int scale) {

}
