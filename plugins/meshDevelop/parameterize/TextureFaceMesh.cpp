#define TEXTUREFACEMESH_CPP
#include "TextureFaceMesh.h"
#include <fstream>
#include <iostream>
#include <YbCore/mesh_proc>
#include <easylogging++.h>
using Plugin::parameterize::indicesTriMesh;

std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> Plugin::parameterize::loadObj(std::string filename) {
    std::ifstream fileHandle(filename,std::ios_base::in);
    std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> result;
    if(!fileHandle.is_open() ) {
        std::cout << "failed to load obj: " << filename << std::endl;
        return result;
    }
    char tmpLine[500];
    indicesTriMesh<glm::vec2>& tmesh = result.first;
    indicesTriMesh<glm::vec3>& vmesh = result.second;
    for(;fileHandle.getline(tmpLine,500);){
        if ( tmpLine[0] == '#' ) continue;
        char *start;
        if((start=strstr(tmpLine,"v "))){
            vmesh.v.resize(vmesh.v.size()+1);
            auto& xyz = *(std::prev(vmesh.v.end()));
            sscanf(start,"v %f%f%f",&xyz[0],&xyz[1],&xyz[2]);
        }if((start=strstr(tmpLine,"vt "))){
            tmesh.v.resize(tmesh.v.size()+1);
            auto& uv = *(std::prev(tmesh.v.end()));
            sscanf(start,"vt %f%f",&uv[0],&uv[1]);
        }else if((start=strstr(tmpLine,"f "))){
            tmesh.f.resize(tmesh.f.size()+1);
            auto& tf = *(std::prev(tmesh.f.end()));
            vmesh.f.resize(vmesh.f.size()+1);
            auto& vf = *(std::prev(vmesh.f.end()));
            sscanf(start,"f %d/%d %d/%d %d/%d",&vf[0],&tf[0],&vf[1],&tf[1],&vf[2],&tf[2]);
            vf -= 1; tf -= 1;
        }
    }
    fileHandle.close();
    return result;
}

namespace _ {
    template <class T>
    void border(indicesTriMesh<T>& mesh) {
        std::map<glm::ivec2, int> map;
    }
}

template<>
void Plugin::parameterize::border<glm::vec2>(indicesTriMesh<glm::vec2>& mesh){
    _::border(mesh);
}
