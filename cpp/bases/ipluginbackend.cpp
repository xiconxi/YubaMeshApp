#include "./IPluginBackend.h"
#include "../mesh/PickableMesh.h"
#include "../mesh/meshProcessing.h"
#include "../mesh/meshCodes.h"
#include "../controller/MeshController.h"
#include "../controller/RenderController.h"

bool IPluginBackend::importMesh(QString url){
    auto str = url.toStdString().substr(7);
    return importMesh(str,str.substr(str.find_last_of("/")));
}

bool IPluginBackend::importMesh(std::string url, std::string name) {
    PickableMesh* mesh = YbCore::IO::readObj(url);
    YbCore::calculateNorm(mesh);
    YbCore::centerlized(mesh);

    RenderScript([=](QTime&) {
        mesh->createBufferScript();
        mesh->syncVertexBuffersDataScript();
        mesh->syncFacesBuffersDataScript();
    });

    con<MeshCtrl>().addMesh(name, mesh); //bunny FullBodyScan 20180205142827.cie

    return true;
}

void IPluginBackend::construction(){}

void IPluginBackend::destruction(){}
