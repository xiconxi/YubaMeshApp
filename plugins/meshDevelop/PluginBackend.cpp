#include "PluginBackend.h"
#include "ScanRender.h"

PluginBackend::PluginBackend()
{

}

void PluginBackend::construction() {
    RenderScript([&](QTime &t) mutable {
        QString prefix = PLUGINPATH"MeshDevelop/shaders/";
        con<ShaderCtrl>().addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
        con<ShaderCtrl>().addShaderProgram("base", shaderConfig{ V(prefix+"indices"),F(prefix+"indices") });
    });
    importMesh(MESHPATH"bunny.obj","scanbody");
    render_s = new("render") ScanRender;
}

void PluginBackend::draw_for(QString value) {

}

template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}
