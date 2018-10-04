//#define MESH_EXPORT_CPP
#include "format_obj.h"
#include <fstream>
#include <easylogging++.h>
#include <iterator>

INITIALIZE_EASYLOGGINGPP

template <class T>
using indicesTriMesh = YbMesh::indicesTriMesh<T>;

indicesTriMesh<glm::vec3> LIBSHARED_EXPORT YbMesh::IO::importOBJ_V0(std::string filename) {
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

std::pair<indicesTriMesh<glm::vec2>,indicesTriMesh<glm::vec3>> LIBSHARED_EXPORT YbMesh::IO::importOBJ_V1(std::string filename) {
    std::ifstream fileHandle(filename,std::ios_base::in);
    auto xyz_faces = std::make_shared<std::vector<glm::ivec3>>();
    auto uv_faces = std::make_shared<std::vector<glm::ivec3>>();
    auto xyzs = std::make_shared<std::vector<glm::vec3>>();
    auto uvs = std::make_shared<std::vector<glm::vec2>>();

    if(!fileHandle.is_open() ) {
        LOG(INFO) << "failed to load obj: " << filename ;
        return std::make_pair( indicesTriMesh<glm::vec2>(uvs, uv_faces), indicesTriMesh<glm::vec3>(xyzs, xyz_faces) );
    }
    char tmpLine[500];
    for(;fileHandle.getline(tmpLine,500);){
        if ( tmpLine[0] == '#' ) continue;
        char *start;
        if((start=strstr(tmpLine,"v "))){
            xyzs->resize(xyzs->size()+1);
            auto& xyz = *(std::prev(xyzs->end()));
            sscanf(start,"v %f%f%f",&xyz[0],&xyz[1],&xyz[2]);
        }else if((start=strstr(tmpLine,"vt "))){
            uvs->resize(uvs->size()+1);
            auto& uv = *(std::prev(uvs->end()));
            sscanf(start,"vt %f%f",&uv[0],&uv[1]);
        }else if((start=strstr(tmpLine,"f "))){
            uv_faces->resize(uv_faces->size()+1);
            auto& tf = *(std::prev(uv_faces->end()));
            xyz_faces->resize(xyz_faces->size()+1);
            auto& vf = *(std::prev(xyz_faces->end()));
            sscanf(start,"f %d/%d %d/%d %d/%d",&vf[0],&tf[0],&vf[1],&tf[1],&vf[2],&tf[2]);
            vf -= 1; tf -= 1;
        }
    }
    fileHandle.close();
    return std::make_pair( indicesTriMesh<glm::vec2>(uvs, uv_faces), indicesTriMesh<glm::vec3>(xyzs, xyz_faces) );;
}

indicesTriMesh<glm::vec3> YbMesh::IO::importOBJ_V2(std::string filename, std::shared_ptr<std::vector<glm::vec2> > uvs){
    std::ifstream fileHandle(filename,std::ios_base::in);
    indicesTriMesh<glm::vec3> vmesh(std::make_shared<std::vector<glm::vec3>>(),
                                    std::make_shared<std::vector<glm::ivec3>>());
    auto faces = vmesh.shared_f();
    auto xyzs  = vmesh.shared_v();
    if(!fileHandle.is_open() ) {
        LOG(INFO) << "failed to load obj: " << filename ;
        return vmesh;
    }
    char tmpLine[500];
    for(;fileHandle.getline(tmpLine,500);){
        if ( tmpLine[0] == '#' ) continue;
        char *start;
        if((start=strstr(tmpLine,"v "))){
            xyzs->resize(xyzs->size()+1);
            auto& xyz = *(std::prev(xyzs->end()));
            sscanf(start,"v %f%f%f",&xyz[0],&xyz[1],&xyz[2]);
        }else if((start=strstr(tmpLine,"vt "))){
            uvs->resize(uvs->size()+1);
            auto& uv = *(std::prev(uvs->end()));
            sscanf(start,"vt %f%f",&uv[0],&uv[1]);
        }else if((start=strstr(tmpLine,"f "))){
            faces->emplace_back(glm::ivec3(0));
            auto& tf = *(std::prev(faces->end()));
            sscanf(start,"f %d %d %d",&tf[0], &tf[1], &tf[2]);
            tf -= 1;
        }
    }
    fileHandle.close();
    return vmesh;
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

void LIBSHARED_EXPORT YbMesh::IO::exportObj(indicesTriMesh<glm::vec3>& mesh, std::string filename) {
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

void LIBSHARED_EXPORT YbMesh::IO::exportObj(indicesTriMesh<glm::vec2>& mesh, std::string filename) {
    std::ofstream file(filename,std::ios_base::out);
    if(!file.is_open() ) {
        std::cout << "failed to export obj: " << filename << std::endl;
    }
    prefix(file, "v") << mesh.v();
    for(auto& e:mesh.f()) e += 1;
    prefix(file, "f") << mesh.f();
    file.close();
}

void LIBSHARED_EXPORT YbMesh::IO::exportObj(std::string filename, std::shared_ptr<std::vector<glm::ivec3>> f,
                                            std::shared_ptr<std::vector<glm::vec3>> v,
                                            std::shared_ptr<std::vector<glm::vec2>> vt) {
    std::ofstream file(filename,std::ios_base::out);
    if(!file.is_open() ) {
        std::cout << "failed to export obj: " << filename << std::endl;
    }
    prefix(file, "v") << *v;
    if(vt.get()) prefix(file, "vt") << *vt;
    for(auto& e:*f) e += 1;
    prefix(file, "f") << *f;
    file.close();
}

void LIBSHARED_EXPORT YbMesh::IO::writePartialMesh(indicesTriMesh<glm::vec3>& mesh,const std::vector<glm::ivec3> &faces, std::string file_name) {
    std::map<int,int> v_rank;
    std::fstream fileHandle;
    fileHandle.open(file_name,std::ofstream::out);
    for(auto it = faces.begin(); it != faces.end(); it++) {
        for(auto& i:{0,1,2}){
            if(v_rank.count((*it)[i])) continue;
            auto& v = mesh.v()[(*it)[i]];
            v_rank[(*it)[i]] = v_rank.size()+1;
            fileHandle << "v " << v[0] << ' ' << v[1] << ' ' << v[2] << std::endl;
        }
        fileHandle << "f " << v_rank[(*it)[0]] << ' ' << v_rank[(*it)[1]] << ' ' << v_rank[(*it)[2]] << std::endl;
    }
    fileHandle.close();
}

void LIBSHARED_EXPORT YbMesh::IO::exportObj(std::string filename, std::shared_ptr<std::vector<glm::ivec3>> f,
                                            std::shared_ptr<std::vector<glm::vec2>> _vt){
    std::ofstream file(filename,std::ios_base::out);
    if(!file.is_open() ) {
        std::cout << "failed to export obj: " << filename << std::endl;
    }
    for(auto& vt: *_vt)
        file << "v " << vt[0] <<' ' << vt[1] << " 0\n";
    for(auto& e:*f) e += 1;
    prefix(file, "f") << *f;
    file.close();
}
