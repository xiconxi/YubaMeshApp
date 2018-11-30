#include "PluginBackend.h"
#include <QStandardPaths>
#include "./render/ComponentsRender.h"
#include "./render/WhirlwindRender.h"
#include "./render/GridTextureRender.h"
#include "./render/PerFaceTexRender.h"


#include <QTcpServer>
#include <QTcpSocket>

#define PI 3.1415926f
PluginBackend::PluginBackend()
{

}

void test(int  s) {
    LOG(INFO) << "AAAA " << s;
}


QTcpServer* tcp_server;
QTcpSocket* tcp_connections;

QTcpSocket* tcp_socket;
void PluginBackend::construction() {
    IPluginBackend::construction();
    RenderScript([&](QTime &t) mutable {
        QString prefix = PLUGINPATH"MeshDevelop/shaders/";
       // con<ShaderCtrl>().addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
        plugin::con<ShaderCtrl>().addShaderProgram({{"base", GLSLFileConfig{ V(prefix+"indices"),F(prefix+"indices") },nullptr}});
        plugin::con<ShaderCtrl>().addShaderProgram({{"grid_tex", GLSLFileConfig{ V(prefix+"grid_tex"),F(prefix+"grid_tex") },nullptr}});
        plugin::con<ShaderCtrl>().addShaderProgram({{"adjacency_tex", GLSLFileConfig{ V(prefix+"adjacency_tex"),G(prefix+"adjacency_tex"),F(prefix+"adjacency_tex") },nullptr}});

    });
    global::con<RenderCtrl>().pause("axes");
    global::con<RenderCtrl>().pause("box");

    tcp_server = new QTcpServer(this);
    tcp_server->listen(QHostAddress::Any, 6843);
    connect(tcp_server,&QTcpServer::newConnection,[=](){
        LOG(INFO) << "new Connection";
        tcp_connections = tcp_server->nextPendingConnection();
        connect(tcp_connections, &QTcpSocket::readyRead, [=](){
            QByteArray qba = tcp_connections->readAll();
            std::cout<< "Recv: "  << qba.toStdString() << std::endl;;
        });
    });

    tcp_socket = new QTcpSocket(this);
    tcp_socket->connectToHost("127.0.0.1",6843);
    tcp_socket->write("1234567890");

    void(*p)(int) = test;
    p(1);

//    importMesh(MESHPATH"bunny.obj","bunny");

//    importMesh(MESHPATH"body2.obj","body2");

//    importMesh("/Users/hotpot/data/mesh_data/2.obj","2.obj");

//    importMesh("/Users/hotpot/data/mesh_data/body2_head.obj","ptex");

//    auto object = con<InteractiveCtrl>().interactiveObject("body2");
//    if(object == nullptr) return;
//    QObject::connect(object,&InteractiveObject::FaceSelected, object, [=](InteractiveObject* mesh){
//        auto select_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/body2_selections.obj";
//        LOG(INFO) << select_file;
//        YbMesh::IO::writePartialMesh(mesh->m_v, mesh->selectedFaces(), select_file);
//    });



}

bool PluginBackend::importMesh(std::string url,std::string name){
    // 人体部件划分
//    components_render = new("Components") ComponentsRender(YbMesh::IO::importOBJ_V0(url),8);
//    plugin::con<InteractiveCtrl>().addInteractiveObject(name, components_render);

    auto mesh = YbMesh::IO::importOBJ_V0(url);
    invincibleWhirlwindTriangle(mesh);
//    invincibleWhirlwindTriangleWithMainDirection(mesh, glm::vec3(0,0,1));
    whirlwind_render = new("Whirwind") WhirlwindRender(std::move(mesh));
    plugin::con<GLMeshCtrl>().addInteractiveObject(name, whirlwind_render);

//    auto mesh = YbMesh::IO::importOBJ_V0(url);
//    gridtexture_render = new ("gridTex") GridTextureRender(mesh);
//    plugin::con<InteractiveCtrl>().addInteractiveObject(name, gridtexture_render);

//    auto mesh = YbMesh::IO::importOBJ_V1(url);
//    perfacetex_render = new ("perfacetex") PerFaceTexRender(mesh.second, mesh.first,url.substr(0, url.find(".obj"))+"Image1.png");
//    plugin::con<GLMeshCtrl>().addInteractiveObject(name, perfacetex_render);

    return true;
}

void PluginBackend::checkComponents(int id, bool status) {
//    if(components_render == nullptr || components_render->visible == false) return;
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
