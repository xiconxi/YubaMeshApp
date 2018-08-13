#include "PluginBackend.h"
#include "ScanRender.h"
#include <YbCore/mesh_proc>
PluginBackend::PluginBackend()
{

}

void PluginBackend::construction() {
    RenderScript([&](QTime &t) mutable {
        QString prefix = PLUGINPATH"MeshDevelop/shaders/";
        con<ShaderCtrl>().addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
        con<ShaderCtrl>().addShaderProgram("base", shaderConfig{ V(prefix+"indices"),F(prefix+"indices") });
     });
     render_s = new("render") ScanRender;
     importMesh(PLUGINPATH"GLViewer/mesh/body2.obj","scanbody");
}

bool PluginBackend::importMesh(std::string url,std::string name) {
    PickableMesh* mesh = YbCore::IO::readObj(url);
    YbCore::calculateNorm(mesh);
    YbCore::centerlized(mesh);

    con<MeshCtrl>().addMesh(name,mesh); //bunny FullBodyScan 20180205142827.cie
    RenderScript([=](QTime &t) {
        mesh->createBufferScript();
        mesh->syncVertexBuffersDataScript();
        mesh->syncFacesBuffersDataScript();
     });
    return true;
}


void PluginBackend::draw_for(QString value) {

}


template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}
