#include "slice.h"
#include <Eigen/Dense>
#include <array>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <easylogging++.h>
void YbMesh::slice::sortByVector(indicesTriMesh<glm::vec3> &mesh, glm::vec3 x) {
    auto& v = mesh.v();
    std::sort(mesh.f().begin(), mesh.f().end(), [=](glm::ivec3& e1, glm::ivec3& e2){
        return glm::dot(v[e1[0]] - v[e2[0]], x) > 0;
    });
}

glm::mat3 YbMesh::slice::pca_analysic(indicesTriMesh<glm::vec3> &mesh, uint r_id) {
    std::array<glm::vec3, 3> pca_vs;
    auto& vs = mesh.v();
    Eigen::MatrixXf X(vs.size(), 3), XX(3,3), vec, val;

    for(size_t i = 0; i < vs.size(); i++) {
        auto& v = vs[i];
        X(i,0) = v[0]; X(i,1) = v[1]; X(i,2) = v[2];
    }

    //normalize
    X.rowwise()  -= X.colwise().mean();
    XX = X.adjoint() * X;

    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eig(XX);

    vec = eig.eigenvectors();
    val = eig.eigenvalues();

//    std::cout << vec << std::endl;
//    std::cout << val << std::endl;

    return glm::make_mat3(vec.data());

//    glm::mat3 coord = glm::make_mat3(vec.data());

//    mesh->model = glm::mat4(glm::transpose(coord))*mesh->model;

//    mesh->model = glm::rotate(glm::mat4(),-3.1415926f*0.5f,glm::vec3(0,0,1))*mesh->model;

//    return coord[r_id];
}

bool YbMesh::slice::isFaceInersected(indicesTriMesh<glm::vec3> &mesh,glm::ivec3 f, glm::vec3 n, float d){
    int flags = ((glm::dot( mesh.v()[f[0]], n ) > d)+(glm::dot( mesh.v()[f[1]], n ) > d)+(glm::dot( mesh.v()[f[2]], n ) > d));
    return flags ==  1 || flags == 2;
}

std::array<glm::vec3,2> YbMesh::slice::getFaceIntersection(indicesTriMesh<glm::vec3> &mesh,glm::ivec3 f, glm::vec3 n, float d){
    std::array<glm::vec3,2> result;
    int size = 0;

    // dot(N, P1) + dot(N, t*(P1-P2) ) = d  // 0 < t < 1
    for(int i:{0,1,2})  {
        double numerator   =  d - glm::dot( mesh.v()[f[i]], n);
        double denominator = glm::dot( mesh.v()[f[i==2?0:i+1]] - mesh.v()[f[i]], n );
        if(abs(denominator) > 1e-6) {
            numerator /= denominator;
            if(numerator >= 0 && numerator <= 1.0) {
//                result[size++] = mesh.v()[f[i]].mv+float(numerator)*(mesh.v()[f[i==2?0:i+1]].mv - mesh.v()[f[i]].mv);
                result[size++] = glm::vec3(f[i],f[i==2?0:i+1],numerator);
            }
        }
    }
    if(size  == 1)
        LOG(INFO) << "vertex intersect!";
    return result;
}

std::array<std::vector<glm::ivec3>::iterator,2> YbMesh::slice::getSliceInterval(indicesTriMesh<glm::vec3> &mesh,glm::vec3 n, float d, float gap) {
    auto& vs = mesh.v();
    std::array<std::vector<glm::ivec3>::iterator,2> result = {
        std::lower_bound(mesh.f().begin(),mesh.f().end(), d-gap,
            [=](const glm::ivec3& e1,const float v){
        return v < glm::dot(vs[e1[0]], n);
    }), std::lower_bound(mesh.f().begin(),mesh.f().end(), d+gap,
            [=](const glm::ivec3& e1,const float v){
        return v < glm::dot(vs[e1[0]], n);
    })};

    if(result[0] > result[1]) std::swap(result[0],result[1]);
    return result;
}
