#include "PluginBackend.h"
#include <QStandardPaths>
#include "ComponentsRender.h"
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
    });
//    importMesh(MESHPATH"bunny.obj","bunny");
    importMesh(MESHPATH"body2.obj","body2");

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

///
/// \brief invincibleWhirlwindTriangle
/// \param mesh
/// 三角网格面元螺旋展开(面元的全排序)
void invincibleWhirlwindTriangle(YbMesh::indicesTriMesh<glm::vec3>& mesh) {
    auto half = YbMesh::SharedHalfEdge(mesh);
}

bool PluginBackend::importMesh(std::string url,std::string name){
    // 人体部件划分
    components_render = new("Components") ComponentsRender(YbMesh::IO::importOBJ_V0(url),8);

    plugin::con<InteractiveCtrl>().addInteractiveObject(name, components_render);

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
