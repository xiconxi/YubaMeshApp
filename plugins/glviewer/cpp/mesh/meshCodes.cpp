#include "meshCodes.h"
#include <iostream>
#include <fstream>
#include <numeric>
#include <glm/gtc/matrix_transform.hpp>
#include <easylogging++.h>

PickableMesh* YbCore::IO::readObj(std::string filename){
    std::ifstream fileHandle(filename,std::ios_base::in);
    if(!fileHandle.is_open() ) {
        std::cout << "failed to load obj: " << filename << std::endl;
        return nullptr;
    }
    PickableMesh* ref = new PickableMesh;
    char tmpLine[500];
    enum F_MODE{V, VT, VTN} f_mode;
    f_mode = F_MODE(VTN + 1);
    for(;fileHandle.getline(tmpLine,500);){
        if ( tmpLine[0] == '#' ) continue;
        char *start;
        if((start=strstr(tmpLine,"v "))){
            ref->v.resize(ref->v.size()+1);
            auto& v = *std::prev(ref->v.end());
            sscanf(start,"v %f%f%f",&v[0],&v[1],&v[2]);
        }else if((start=strstr(tmpLine,"f "))){
            ref->f.resize(ref->f.size()+1);
            auto& f = *std::prev(ref->f.end());
            switch (f_mode) {
            case VTN:
                sscanf(start,"f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",&f[0],&f[1],&f[2]);
                    break;
            case VT:
                sscanf(start,"f %d/%*d %d/%*d %d/%*d",&f[0],&f[1],&f[2]);
                    break;
            case V:
                sscanf(start,"f %d %d %d",&f[0],&f[1],&f[2]);
                    break;
            default:
                if(sscanf(start,"f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",&f[0],&f[1],&f[2])==3)
                    f_mode = VTN;
                else if(sscanf(start,"f %d/%*d %d/%*d %d/%*d",&f[0],&f[1],&f[2])==3)
                    f_mode = VT;
                else if(sscanf(start,"f %d %d %d",&f[0],&f[1],&f[2])==3)
                    f_mode = V;
                break;
            }
            f -= 1;
        }
    }
    ref->vn.resize(ref->v.size());
    LOG(INFO) << filename << " : " << ref << ' ' << ref->v.size() << ' ' << ref->f.size();
    fileHandle.close();
    return ref;
}

PickableMesh* YbCore::IO::readOff(std::string filename) {
    std::fstream fileHandle;
    fileHandle.open(filename,std::ifstream::in);
    if(!fileHandle.is_open()) {
        std::cout << "failed to load off: " << filename;
        return nullptr;
    }
    auto ref = new PickableMesh();
    int size[3];
    char tmpLine[300];
    fileHandle.getline(tmpLine,300);
    if(strstr(tmpLine, "OFF") == NULL){
        std::cout << "failed to load off: " << filename;
        return nullptr;
    }
    fileHandle.getline(tmpLine,300);
    sscanf(tmpLine,"%d %d %d",size, size+1, size+2);
    ref->v.resize(size[0]);
    ref->v.resize(size[0]);
    ref->f.resize(size[1]);
    for(int i=0; i< size[0]; i++) {
        fileHandle.getline(tmpLine,300);
        sscanf(tmpLine,"%f%f%f",&ref->v[i][0],&ref->v[i][1],&ref->v[i][2]);
    }

    for(int i=0; i< size[1]; i++) {
        fileHandle.getline(tmpLine,300);
        sscanf(tmpLine,"3 %d %d %d",&ref->f[i][0],&ref->f[i][1],&ref->f[i][2]);
    }
    fileHandle.close();
    return ref;
}

PickableMesh* YbCore::IO::readCIE(std::string filename) {
//    std::ifstream file;
//    file.open(filename, std::ifstream::binary);
//    if (!file.is_open()) return false;
//    unsigned int v_size[4] = {};
//    file.read((char *) v_size, 4 * sizeof(unsigned int));
//    std::vector<std::array<glm::vec2,8> > verts;
//    std::vector<glm::ivec4 > faces;
//    std::vector<glm::vec4 > faces_ex;
//    verts.resize(v_size[0]);
//    faces.resize(v_size[1]);
//    faces_ex.resize(v_size[2]);

//    ref->v.resize(verts.size());
//    ref->vts.resize(verts.size());
//    ref->f.resize(faces.size());
//    file.read((char *) verts.data(), verts.size() * sizeof(verts[0]));
//    file.read((char *) faces.data(), faces.size() * sizeof(faces[0]));
//    file.read((char *) faces_ex.data(), faces_ex.size() * sizeof(faces_ex[0]));
//    file.close();
//    LOG(INFO) << verts[0][1][1];

//    for(int i = 0;i < faces.size(); i++)
//        ref->f[i] = glm::ivec3(faces[i]);

//    for(int i = 0;i < ref->v.size(); i++){
//        ref->v[i].mv = glm::vec3(verts[i][0],verts[i][1][0]);
//        ref->vts[i][0] = glm::vec3(verts[i][4],0);
//        ref->vts[i][1] = glm::vec3(verts[i][5],0);
//        ref->vts[i][2] = glm::vec3(verts[i][6],0);
//        ref->vts[i][3] = glm::vec3(verts[i][7],0);
//    }
//    std::vector<glm::vec2> alpha(ref->v.size(), glm::vec2(0) );
//    for(int j = 0 ; j < ref->vts[0].size(); j++){
//        for(int i = 0; i < ref->f.size(); i++){
//            auto& e = ref->f[i];
//            for(int t = 0; t< 3; t++ ) {
//                alpha[e[t]] += glm::vec2(faces_ex[i][j],1.0f);
//            }
//        }
//        for(int i = 0 ; i < ref->vts.size(); i++) {
//            ref->vts[i][j][2] = alpha[i][0]/alpha[i][1];
//        }
//        std::fill(alpha.begin(), alpha.end(), glm::vec2(0));
//    }
//    for(auto& e:ref->vts){
//        float alpha_sum = std::accumulate(e.begin(), e.end(), 0.0f, [=](float s,glm::vec3& e1){
//            return s + e1[2];
//        });

//        std::for_each(e.begin(), e.end(), [=](glm::vec3& e1){
//            if(alpha_sum < 1e-1) return;
//            if(alpha_sum > 1e2) return;
//            e1[2] /= alpha_sum;
//        });
//    }
    return nullptr;
}
/*
void writeObj(std::string filename){
    std::fstream fileHandle;
    fileHandle.open(filename,std::ifstream::out);
    if(!fileHandle.is_open()) {
        std::cout << "failed to write obj: " << filename;
        return;
    }
    fileHandle<< "# v: " << _v.size() << "   f: " << _f.size() << '\n';
    for(int i=0;i< _v.size(); i++)
        fileHandle <<"v " <<_v[i].mv[0] << ' ' << _v[i].mv[1] << ' ' << _v[i].mv[2] << '\n';
    for(int i=0;i< _v.size(); i++)
        fileHandle <<"vn " <<_v[i].mn[0] << ' ' << _v[i].mn[1] << ' ' << _v[i].mn[2] << '\n';
    for(int i=0;i< _f.size(); i++)
        fileHandle <<"f " <<_f[i][0]+1 << ' ' << _f[i][1]+1 << ' ' << _f[i][2]+1 << '\n';
    fileHandle.close();
}

void writeOff(std::string filename){
    std::fstream fileHandle;
    fileHandle.open(filename,std::ifstream::out);
    if(!fileHandle.is_open()) {
        std::cout << "failed to write off: " << filename;
        return;
    }
    fileHandle<< "OFF\ " << _v.size() << "  " << _f.size() << '0\n';
    for(int i=0;i< _v.size(); i++)
        fileHandle <<_v[i].mv[0] << ' ' << _v[i].mv[1] << ' ' << _v[i].mv[2] << '\n';
    for(int i=0;i< _f.size(); i++)
        fileHandle <<"3 " <<_f[i][0]+1 << ' ' << _f[i][1]+1 << ' ' << _f[i][2]+1 << '\n';
    fileHandle.close();
}
*/
