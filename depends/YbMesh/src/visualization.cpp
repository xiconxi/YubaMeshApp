#include "visualization.h"
#include <numeric>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void LIBSHARED_EXPORT YbMesh::visualization::calculateNorm(indicesTriMesh<glm::vec3>& vMesh, indicesTriMesh<glm::vec3>& nMesh){
    auto& v = vMesh.v();
    auto& vn = nMesh.v();
    auto& f = vMesh.f();
    if(vn.size() != v.size())
        vn.resize(v.size());
    std::vector<float> acc(v.size(),0);
    std::vector<glm::vec3> norms(v.size(), glm::vec3(0));
    for(int i=0; i< f.size(); i++) {
        glm::vec3 norm = glm::cross(v[f[i][0]]-v[f[i][1]],
                                    v[f[i][2]]-v[f[i][1]]);
        for(int j=0; j< 3; j++){
            vn[f[i][j]] += norm ;
            acc[f[i][j]] ++;
        }
    }
    for(int i=0; i< v.size(); i++){
        vn[i] = glm::normalize( vn[i]/acc[i] );
    }
}

glm::mat4 LIBSHARED_EXPORT YbMesh::visualization::normalize(indicesTriMesh<glm::vec3>& vMesh, bool centerlized){
    auto& v = vMesh.v();
    glm::vec3 center = std::accumulate(v.begin(), v.end(), glm::vec3(0),[=](glm::vec3 acc,glm::vec3& e){
        return acc+e;
    })/(float)v.size();
    if(centerlized)
        for(auto& e:v) e -= center;
//    return  glm::scale(glm::mat4(), glm::vec3(1.4 / scale_k))*glm::translate(glm::mat4(), -center);

    return  glm::scale(glm::mat4(), glm::vec3(1.4 / std::sqrt(std::accumulate(v.begin(), v.end(), 0.0,[=](float s, glm::vec3& e){
        return std::max<float>(s,std::powf(glm::length(centerlized?e:e-center),2) );
    }))));
}
