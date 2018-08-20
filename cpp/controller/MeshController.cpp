#include "MeshController.h"
#include <glm/gtc/type_ptr.hpp>
#include "RenderController.h"
#include "../FboNode.h"
#include "../utils/Select.h"
void MeshCtrl::addMesh(std::string name, PickableMesh *mesh) {
    if(meshes.find(name) != meshes.end())
        delete meshes[name];
    meshes[name] = mesh;
    setSelectableMesh(mesh);

    con<CentralCtrl>().selectTool->sizingBufferScriptWrap(mesh->f.size());
    con<CentralCtrl>().resourceHook(CentralCtrl::MESH,name);
}

void MeshCtrl::releaseMesh(std::string name) {
    delete meshes[name];
    meshes.erase(meshes.find(name));
}

PickableMesh* MeshCtrl::mesh(std::string name){
    auto result = meshes[name];
    if(result == nullptr) LOG(INFO) << "MeshCtrl: invalid " << name;
    return result;
}

PickableMesh* MeshCtrl::mesh(uint id){
    return (*std::find_if(meshes.begin(), meshes.end(),[=](const std::pair<std::string,PickableMesh*>& e){
        return e.second->numericalId() == id;
    })).second;
}

PickableMesh* MeshCtrl::selectable() {
    return selectable_mesh;
}

void MeshCtrl::setSelectableMesh(uint id){
    setSelectableMesh(mesh(id));
}

void MeshCtrl::setSelectableMesh(PickableMesh* mesh){
    selectable_mesh = mesh;
}

void MeshCtrl::setSelectableMesh(std::string name){
    setSelectableMesh(mesh(name));
}

template<>
LIBSHARED_EXPORT MeshCtrl& con<MeshCtrl>(){
    return ICtrl<MeshCtrl>::getInstanceRef();
}
