#include "PluginBackend.h"
#include "ScanRender.h"
#include <mesh_proc>
PluginBackend::PluginBackend()
{

}

void PluginBackend::construction() {
    QString prefix = "../PlugIns/MeshDevelop/shaders/";
    RenderScript([=](QTime &t) mutable {
        con<ShaderCtrl>().addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
        con<ShaderCtrl>().addShaderProgram("base", shaderConfig{ V(prefix+"indices"),F(prefix+"indices") });
        PickableMesh* mesh = readObj("../PlugIns/GLViewer/mesh/body2.obj");
        YbCore::calculateNorm(mesh);
        YbCore::centerlized(mesh);
        render_s->orient = YbCore::pca_analysic(mesh,2);
        YbCore::sortByVector(mesh, render_s->orient);
        mesh->createBuffers();
        mesh->syncVertexBuffersData();
        mesh->syncFacesBuffersData();
        con<MeshCtrl>().addMesh("scanbody",mesh); //bunny FullBodyScan 20180205142827.cie
    });
    render_s = new("render") ScanRender;
}

void PluginBackend::draw_for(QString value) {
    if(value == "mesh") {
        render_s->setRender(std::bind(&ScanRender::draw_model,render_s,std::placeholders::_1));
    }
    //    new ("texMix") RenderScript([=](RenderScript* rf){
    //        TGL.glEnable(GL_DEPTH_TEST);
    //        TGL.glClearColor(0.2, 0.3, 0.3, 1.0);
    //        TGL.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //        auto shader = SMM("texture");
    //        auto mesh = TMM("bunny");
    //        if(mesh->texture)
    //            mesh->texture->bind();
    //        shader->bind();
    //        mesh->indicesDraw();
    //        shader->release();
    //        LOG(INFO) << "texture rendering. ";
    //    });
}


template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}
