#include "./meshProcessing.h"
#include <algorithm>
#include <numeric>
#include <easylog/easylogging++.h>
#include <Eigen/Dense>

void sortByVector(glFormatMesh *mesh, glm::vec3 x) {
    std::sort(mesh->f.begin(), mesh->f.end(), [=](glm::ivec3& e1, glm::ivec3& e2){
        return glm::dot(mesh->v[e1[0]].mv - mesh->v[e2[0]].mv, x) > 0;
    });
}

bool slice_in_triangel(glFormatMesh* mesh,glm::ivec3 f, glm::vec3 n, float d){
    int flags = ((glm::dot( mesh->v[f[0]].mv, n ) > d)+(glm::dot( mesh->v[f[1]].mv, n ) > d)+(glm::dot( mesh->v[f[2]].mv, n ) > d));
    return flags ==  1 || flags == 2;
}

std::array<glm::vec3,2> slice_triangel_line(glFormatMesh* mesh,glm::ivec3 f, glm::vec3 n, float d){
    std::array<glm::vec3,2> result;
    int size = 0;
    // dot(N, P1) + dot(N, t*(P1-P2) ) = d  // 0 < t < 1
    for(int i:{0,1,2})  {
        double numerator   =  d - glm::dot( mesh->v[f[i]].mv, n);
        double denominator = glm::dot( mesh->v[f[i==2?0:i+1]].mv - mesh->v[f[i]].mv, n );
        if(abs(denominator) > 1e-6) {
            numerator /= denominator;
            if(numerator >= 0 && numerator <= 1.0) {
                result[size++] = mesh->v[f[i]].mv+float(numerator)*(mesh->v[f[i==2?0:i+1]].mv - mesh->v[f[i]].mv);
            }
        }
    }
    return result;
}

