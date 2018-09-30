#include "./IPluginBackend.h"
#include "./InteractiveObjectMesh.h"
#include "../controller/RenderController.h"
#include "../controller/ShaderController.h"
#include "../controller/InteractiveController.h"
#include <YbMesh/YbMesh.hpp>

bool IPluginBackend::importMesh(QString url){
    auto str = url.toStdString().substr(7);
    return importMesh(str,str.substr(str.find_last_of("/")));
}

bool IPluginBackend::importMesh(std::string url, std::string name) {
    auto object = new InteractiveObject(YbMesh::IO::importOBJ_V0(url));
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
        object->syncSelectBufferScript();
    });
    global::con<InteractiveCtrl>().addInteractiveObject(name, object);
    return true;
}

void IPluginBackend::construction(){
   ICtrl<ShaderCtrl>::ReleasePluginInstance();
   ICtrl<InteractiveCtrl>::ReleasePluginInstance();
}

void IPluginBackend::destruction(){

}
