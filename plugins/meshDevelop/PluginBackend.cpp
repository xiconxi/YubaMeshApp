#include "PluginBackend.h"
#include <QStandardPaths>
#include "./render/ComponentsRender.h"
#include "./render/WhirlwindRender.h"
#include "./render/GridTextureRender.h"

#define PI 3.1415926f
PluginBackend::PluginBackend()
{

}

void PluginBackend::construction() {
    IPluginBackend::construction();
    RenderScript([&](QTime &t) mutable {
        QString prefix = PLUGINPATH"MeshDevelop/shaders/";
       // con<ShaderCtrl>().addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
        plugin::con<ShaderCtrl>().addShaderProgram({{"base", GLSLFileConfig{ V(prefix+"indices"),F(prefix+"indices") },nullptr}});
        plugin::con<ShaderCtrl>().addShaderProgram({{"grid_tex", GLSLFileConfig{ V(prefix+"grid_tex"),F(prefix+"grid_tex") },nullptr}});
    });
//    importMesh(MESHPATH"bunny.obj","bunny");
//    importMesh(MESHPATH"body2.obj","body2");

    importMesh("/Users/hotpot/data/mesh_data/body2_head.obj","ptex");

//    if(render_s == nullptr)
//        render_s = new("render") ScanRender;
//    auto object = con<InteractiveCtrl>().interactiveObject("body2");
//    if(object == nullptr) return;
//    QObject::connect(object,&InteractiveObject::FaceSelected, object, [=](InteractiveObject* mesh){
//        auto select_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/body2_selections.obj";
//        LOG(INFO) << select_file;
//        YbMesh::IO::writePartialMesh(mesh->m_v, mesh->selectedFaces(), select_file);
//    });
}

void PluginBackend::draw_for(QString value) {

}

bool PluginBackend::importMesh(std::string url,std::string name){
    // 人体部件划分
//    components_render = new("Components") ComponentsRender(YbMesh::IO::importOBJ_V0(url),8);

//    plugin::con<InteractiveCtrl>().addInteractiveObject(name, components_render);

//    auto mesh = YbMesh::IO::importOBJ_V0(url);
//    invincibleWhirlwindTriangle(mesh);
//    whirlwind_render = new("Whirwind") WhirlwindRender(std::move(mesh));
//    plugin::con<InteractiveCtrl>().addInteractiveObject(name, whirlwind_render);

    auto mesh = YbMesh::IO::importOBJ_V0(url);
    gridtexture_render = new ("gridTex") GridTextureRender(mesh);
    plugin::con<InteractiveCtrl>().addInteractiveObject(name, gridtexture_render);

    global::con<InteractiveCtrl>().object("axes")->visible = false;
    global::con<InteractiveCtrl>().object("box")->visible = false;

    return true;
}

void PluginBackend::checkComponents(int id, bool status) {
    if(components_render == nullptr || components_render->visible == false) return;
    components_render->components.intervals[id][1] = (status?1:-1)*abs(components_render->components.intervals[id][1]);
    components_render->components.update();
    global::con<RenderCtrl>().update();
}

template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}
