#include "PluginBackend.h"
#include "ScanRender.h"
#include <YbCore/mesh_proc>
#include "parameterize/TextureFaceMesh.h"
#include "parameterize/ConnectedComponentAnalysis.h"

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
    importMesh(PLUGINPATH"GLViewer/mesh/body1.obj","scanbody");
}

bool PluginBackend::importMesh(std::string url,std::string name) {
//    con<MeshCtrl>().mesh("bunny")->visible = false;
//    PickableMesh* mesh = YbCore::IO::readObj(url);
    PickableMesh* mesh = new PickableMesh();
    auto result = Plugin::parameterize::loadObj(url);

    mesh->v = result.second.v;
    mesh->f = result.second.f;
    YbCore::calculateNorm(mesh);
    YbCore::centerlized(mesh);
    con<MeshCtrl>().addMesh(name,mesh); //bunny FullBodyScan 20180205142827.cie
    RenderScript([=](QTime &t) {
        mesh->createBufferScript();
        mesh->syncVertexBuffersDataScript();
        mesh->syncFacesBuffersDataScript();
    });

//    Plugin::parameterize::border(result.first);

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
