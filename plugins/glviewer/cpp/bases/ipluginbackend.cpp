#include "./IPluginBackend.h"
#include "../mesh/PickableMesh.h"
#include "../controller/RenderController.h"
#include "../controller/InteractiveController.h"
#include <YbMesh/YbMesh.hpp>

bool IPluginBackend::importMesh(QString url){
    auto str = url.toStdString().substr(7);
    return importMesh(str,str.substr(str.find_last_of("/")));
}

bool IPluginBackend::importMesh(std::string url, std::string name) {
    YbMesh::indicesTriMesh<glm::vec3> triMesh = YbMesh::IO::importOBJ_V0(url);
    auto object = new InteractiveObject(triMesh,YbMesh::indicesTriMesh<glm::vec3>(std::make_shared<std::vector<glm::vec3>>(),triMesh.f()));
    object->centerlized();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    con<InteractiveCtrl>().addInteractiveObject(name, object);
    return true;
}

void IPluginBackend::construction(){}

void IPluginBackend::destruction(){}
