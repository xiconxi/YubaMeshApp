#include "meshProcessing.h"
#include <easylogging++.h>
#include <Eigen/Dense>
#include <glm/gtc/type_ptr.hpp>

void YbCore::centerlized(PickableMesh* ref){
    glm::vec3 center = std::accumulate(ref->v.begin(), ref->v.end(), glm::vec3(0),[=](glm::vec3 acc,glm::vec3& e){
        return acc+e;
    })/(float)ref->v.size();
    float scale_k = std::sqrt(std::accumulate(ref->v.begin(), ref->v.end(), 0.0, [=](float s, glm::vec3& e){
        return std::max<float>(s,glm::length(e-center)* glm::length(e-center));
    })); // /(float)_v.size())

    ref->model = glm::scale(glm::mat4(), glm::vec3(1.4 / scale_k))*glm::translate(glm::mat4(), -center);
}

void YbCore::calculateNorm(PickableMesh* ref){
    if(ref->vn.size() != ref->v.size())
        ref->vn.resize(ref->v.size());
    std::vector<float> acc(ref->v.size(),0);
    std::vector<glm::vec3> norms(ref->v.size(), glm::vec3(0));
    for(int i=0; i< ref->f.size(); i++) {
        glm::vec3 norm = glm::cross(ref->v[ref->f[i][0]]-ref->v[ref->f[i][1]],
                                    ref->v[ref->f[i][2]]-ref->v[ref->f[i][1]]);
        for(int j=0; j< 3; j++){
            ref->vn[ref->f[i][j]] += norm ;
            acc[ref->f[i][j]] ++;
        }
    }
    for(int i=0; i< ref->v.size(); i++){
        ref->vn[i] = glm::normalize( ref->vn[i]/acc[i] );
    }
}

void YbCore::sortByVector(PickableMesh *mesh, glm::vec3 x) {
    std::sort(mesh->f.begin(), mesh->f.end(), [=](glm::ivec3& e1, glm::ivec3& e2){
        return glm::dot(mesh->v[e1[0]] - mesh->v[e2[0]], x) > 0;
    });
}

bool YbCore::isFaceInersected(PickableMesh* mesh,glm::ivec3 f, glm::vec3 n, float d){
    int flags = ((glm::dot( mesh->v[f[0]], n ) > d)+(glm::dot( mesh->v[f[1]], n ) > d)+(glm::dot( mesh->v[f[2]], n ) > d));
    return flags ==  1 || flags == 2;
}

std::array<glm::vec3,2> YbCore::getFaceIntersection(PickableMesh* mesh,glm::ivec3 f, glm::vec3 n, float d){
    std::array<glm::vec3,2> result;
    int size = 0;

    // dot(N, P1) + dot(N, t*(P1-P2) ) = d  // 0 < t < 1
    for(int i:{0,1,2})  {
        double numerator   =  d - glm::dot( mesh->v[f[i]], n);
        double denominator = glm::dot( mesh->v[f[i==2?0:i+1]] - mesh->v[f[i]], n );
        if(abs(denominator) > 1e-6) {
            numerator /= denominator;
            if(numerator >= 0 && numerator <= 1.0) {
//                result[size++] = mesh->v[f[i]].mv+float(numerator)*(mesh->v[f[i==2?0:i+1]].mv - mesh->v[f[i]].mv);
                result[size++] = glm::vec3(f[i],f[i==2?0:i+1],numerator);
            }
        }
    }
    if(size  == 1)
        LOG(INFO) << "vertex intersect!";
    return result;
}

std::array<std::vector<glm::ivec3>::iterator,2> YbCore::getSliceInterval(PickableMesh* mesh,glm::vec3 n, float d, float gap) {
    std::array<std::vector<glm::ivec3>::iterator,2> result = {
        std::lower_bound(mesh->f.begin(),mesh->f.end(), d-gap,
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]], n);
    }), std::lower_bound(mesh->f.begin(),mesh->f.end(), d+gap,
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]], n);
    })};

    if(result[0] > result[1]) std::swap(result[0],result[1]);
    return result;
}

glm::vec3 YbCore::pca_analysic(PickableMesh *mesh, uint r_id) {
    std::array<glm::vec3, 3> pca_vs;
    Eigen::MatrixXf X(mesh->v.size(), 3), XX(3,3), vec, val;

    for(size_t i = 0; i < mesh->v.size(); i++) {
        auto& v = mesh->v[i];
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

    glm::mat3 coord = glm::make_mat3(vec.data());

    mesh->model = glm::mat4(glm::transpose(coord))*mesh->model;

    mesh->model = glm::rotate(glm::mat4(),-3.1415926f*0.5f,glm::vec3(0,0,1))*mesh->model;

    return coord[r_id];
}


