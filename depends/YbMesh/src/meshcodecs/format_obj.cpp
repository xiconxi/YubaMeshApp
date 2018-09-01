#define MESH_EXPORT_CPP
#include "format_obj.h"
#include <fstream>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

template <class T>
using indicesTriMesh = YbMesh::indicesTriMesh<T>;

indicesTriMesh<glm::vec3> YbMesh::IO::importOBJ_V0(std::string filename) {
    std::ifstream fileHandle(filename,std::ios_base::in);
    indicesTriMesh<glm::vec3> vmesh(std::make_shared<std::vector<glm::vec3>>(),
                                    std::make_shared<std::vector<glm::ivec3>>());
    if(!fileHandle.is_open() ) {
        LOG(INFO) << "failed to load obj: " << filename;
        return vmesh;
    }
    char tmpLine[500];
    enum F_MODE{V, VT, VTN} f_mode;
    f_mode = F_MODE(VTN + 1);
    for(;fileHandle.getline(tmpLine,500);){
        if ( tmpLine[0] == '#' ) continue;
        char *start;
        if((start=strstr(tmpLine,"v "))){
            vmesh.v().resize(vmesh.v().size()+1);
            auto& xyz = *(std::prev(vmesh.v().end()));
            sscanf(start,"v %f%f%f",&xyz[0],&xyz[1],&xyz[2]);
        }else if((start=strstr(tmpLine,"f "))){
            vmesh.f().resize(vmesh.f().size()+1);
            auto& f = *(std::prev(vmesh.f().end()));
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
    LOG(INFO) << filename << ' ' << vmesh.v().size() << ' ' << vmesh.f().size();
    fileHandle.close();
    return vmesh;
}

std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> YbMesh::IO::importOBJ_V1(std::string filename) {
    std::ifstream fileHandle(filename,std::ios_base::in);
    auto& xyz_faces = *(new std::vector<glm::ivec3>());
    auto& uv_faces = *(new std::vector<glm::ivec3>());
    auto& xyzs = *(new std::vector<glm::vec3>());
    auto& uvs = *(new std::vector<glm::vec2>());

    if(!fileHandle.is_open() ) {
        LOG(INFO) << "failed to load obj: " << filename ;
        return std::make_pair( indicesTriMesh<glm::vec2>(uvs, uv_faces), indicesTriMesh<glm::vec3>(xyzs, xyz_faces) );
    }
    char tmpLine[500];
    for(;fileHandle.getline(tmpLine,500);){
        if ( tmpLine[0] == '#' ) continue;
        char *start;
        if((start=strstr(tmpLine,"v "))){
            xyzs.resize(xyzs.size()+1);
            auto& xyz = *(std::prev(xyzs.end()));
            sscanf(start,"v %f%f%f",&xyz[0],&xyz[1],&xyz[2]);
        }else if((start=strstr(tmpLine,"vt "))){
            uvs.resize(uvs.size()+1);
            auto& uv = *(std::prev(uvs.end()));
            sscanf(start,"vt %f%f",&uv[0],&uv[1]);
        }else if((start=strstr(tmpLine,"f "))){
            uv_faces.resize(uv_faces.size()+1);
            auto& tf = *(std::prev(uv_faces.end()));
            xyz_faces.resize(xyz_faces.size()+1);
            auto& vf = *(std::prev(xyz_faces.end()));
            sscanf(start,"f %d/%d %d/%d %d/%d",&vf[0],&tf[0],&vf[1],&tf[1],&vf[2],&tf[2]);
            vf -= 1; tf -= 1;
        }
    }
    fileHandle.close();
    return std::make_pair( indicesTriMesh<glm::vec2>(uvs, uv_faces), indicesTriMesh<glm::vec3>(xyzs, xyz_faces) );;
}

std::string mesh_prefix = "";

std::ostream& prefix(std::ostream& os,std::string x){
    mesh_prefix = x + ' ';
    return os;
}

template <typename T>
std::ostream& operator << (std::ostream &os, std::vector<T> &faces){
    os << "#" << mesh_prefix << " " << faces.size() << '\n' << mesh_prefix;
    std::copy(faces.begin(), faces.end(), std::ostream_iterator<T>(os, ('\n'+mesh_prefix).data()));
    os.seekp(-mesh_prefix.size(), std::ios_base::end);
    return os << std::endl; // std::string(mesh_prefix.size(),'\b')+std::string(mesh_prefix.size(),' ')
}

void YbMesh::IO::exportObj(indicesTriMesh<glm::vec3>& mesh, std::string filename) {
//    if(mesh.v().size() == 0 || mesh.f().size() == 0) {
//        std::cout << "export target is empty: " << filename  << std::endl;
//        return ;
//    }
    std::ofstream file(filename,std::ios_base::out);
    if(!file.is_open() ) {
        std::cout << "failed to export obj: " << filename << std::endl;
        return ;
    }
    prefix(file, "v") << mesh.v();
    for(auto& e:mesh.f()) e += 1;
    prefix(file, "f") << mesh.f();
    file.close();
}

void YbMesh::IO::exportObj(indicesTriMesh<glm::vec2>& mesh, std::string filename) {
    std::ofstream file(filename,std::ios_base::out);
    if(!file.is_open() ) {
        std::cout << "failed to export obj: " << filename << std::endl;
    }
    prefix(file, "v") << mesh.v();
    for(auto& e:mesh.f()) e += 1;
    prefix(file, "f") << mesh.f();
    file.close();
}
