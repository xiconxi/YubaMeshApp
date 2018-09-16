#include "PluginBackend.h"
#include "ScanRender.h"
#include <QStandardPaths>
#define PI 3.1415926f
PluginBackend::PluginBackend()
{

}

void VertexClusterMeshObject::syncVertexBuffersDataScript() {
    auto& f = m_v.f();
    auto& v = m_v.v();
    auto& vn = m_n.v();
    auto& vc = vertex_cluster;
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    gl.glBufferData(GL_ARRAY_BUFFER, v.size()*(sizeof(v[0])+sizeof(vn[0])+sizeof(vc[0])), nullptr, GL_STATIC_DRAW);

    gl.glBufferSubData(GL_ARRAY_BUFFER, 0, v.size()*sizeof(v[0]), v.data());
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v[0]), (GLvoid*)0);
    gl.glEnableVertexAttribArray(0);

    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), vn.size()*sizeof(vn[0]), vn.data());
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vn[0]), (GLvoid*)(v.size()*sizeof(v[0])));
    gl.glEnableVertexAttribArray(1);

    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0])+vn.size()*sizeof(vn[0]), vc.size()*sizeof(vc[0]), vc.data());
    gl.glVertexAttribIPointer(2, 1, GL_INT, sizeof(vc[0]), (GLvoid*)(v.size()*sizeof(v[0]) + vn.size()*(sizeof(vn[0]))));
    gl.glEnableVertexAttribArray(2);

    gl.glBindVertexArray(0);
}

VertexClusterMeshObject::VertexClusterMeshObject(TriMesh vmesh,TriMesh nmesh,int components)
    :IDrawObject(vmesh,nmesh,components),vertex_cluster(vmesh.v().size(),0){
}

void PluginBackend::construction() {
    RenderScript([&](QTime &t) mutable {
        QString prefix = PLUGINPATH"MeshDevelop/shaders/";
        con<ShaderCtrl>().addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
        con<ShaderCtrl>().addShaderProgram("base", shaderConfig{ V(prefix+"indices"),F(prefix+"indices") });
    });

//    importMesh(MESHPATH"bunny.obj","bunny");
    importMesh(MESHPATH"body2.obj","body2");
    render_s = new("render") ScanRender;
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
    YbMesh::indicesTriMesh<glm::vec3> triMesh = YbMesh::IO::importOBJ_V0(url);
    auto object = new VertexClusterMeshObject(triMesh,YbMesh::indicesTriMesh<glm::vec3>(std::make_shared<std::vector<glm::vec3>>(),triMesh.f()),8);
    object->centerlized();
    object->calculateNorm();

    glm::mat3 pca = YbMesh::slice::pca_analysic(triMesh.v(), triMesh.f().begin(), triMesh.f().end());
    YbMesh::slice::sortByVector(triMesh, pca[2]);

    std::array<float,2> Z = {glm::dot(pca[2],triMesh.v()[(*std::prev(triMesh.f().end())).r]),glm::dot(pca[2],triMesh.v()[(*triMesh.f().begin()).r])};
    std::vector<float> z_dots(triMesh.v().size());
    for(int i = 0; i < triMesh.v().size(); i++)
        z_dots[i] = glm::dot(triMesh.v()[i], pca[2]);

    double mean_dis = std::sqrt(std::accumulate(triMesh.v().begin(),triMesh.v().end(), 0.0,
                    [=](double acc,glm::vec3& e){
                        return acc+std::powf(glm::length(e-pca[2]*glm::dot(e, pca[2]) ),2);
                                             })/triMesh.v().size());

    std::vector<std::array<float,2>> intervals = {
        {glm::mix(Z[0],Z[1],0.00),glm::mix(Z[0],Z[1],0.42)}, // 腿
        {glm::mix(Z[0],Z[1],0.42),glm::mix(Z[0],Z[1],0.54)},
        {glm::mix(Z[0],Z[1],0.54),glm::mix(Z[0],Z[1],0.74)}, // 躯干
        {glm::mix(Z[0],Z[1],0.74),glm::mix(Z[0],Z[1],0.84)}, // 肩膀
        {glm::mix(Z[0],Z[1],0.84),glm::mix(Z[0],Z[1],1.00)}, // 头
    };

    for(int i = 0; i < triMesh.v().size(); i++) {
        if(z_dots[i] >= (*intervals.begin())[0] && z_dots[i] <= (*intervals.begin())[1]) {
            object->vertex_cluster[i] = glm::dot(triMesh.v()[i], pca[1]) > 0? 2:1 ;
        }else {
            if( glm::length(triMesh.v()[i]-pca[2]*z_dots[i] ) > mean_dis ){
                object->vertex_cluster[i] = glm::dot(triMesh.v()[i], pca[1]) >0 ?4:3;
            }
            else for(auto it = intervals.begin() + 1; it != intervals.end(); it++) {
                if(z_dots[i] >= (*it)[0] && z_dots[i] <= (*it)[1]) {
                    object->vertex_cluster[i] = 4 + it - intervals.begin() ;
                }
            }
        }
    }
    std::sort(triMesh.f().begin(), triMesh.f().end(), [object,z_dots](const glm::vec3& e1, const glm::vec3& e2){
        return object->vertex_cluster[e1.r] == object->vertex_cluster[e2.r] ?
               z_dots[e1.r] < z_dots[e2.r]:object->vertex_cluster[e1.r] < object->vertex_cluster[e2.r];
    });
    for(int i = 0, offset = 0; i < 8; i++) {
        object->components.intervals[i][0] = offset;
        object->components.intervals[i][1] = std::lower_bound(triMesh.f().begin(), triMesh.f().end(), i+2 ,[object](const glm::vec3& e1, const int v){
            return object->vertex_cluster[e1.r] < v;
        }) - triMesh.f().begin();
        offset = object->components.intervals[i][1];
    }
    object->components.update();

    object->model = glm::rotate(glm::mat4(),-3.1415926f*0.5f,glm::vec3(0,0,1))* glm::mat4(glm::transpose(pca)) * object->model;

    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    con<InteractiveCtrl>().addInteractiveObject(name, object);
    return true;
}

void PluginBackend::checkComponents(int id, bool status) {
    for(auto objectKV:con<InteractiveCtrl>().allObjects()){
        auto mesh = dynamic_cast<VertexClusterMeshObject*>(objectKV.second);
        if(mesh == nullptr || mesh->visible == false) continue;
        mesh->components.intervals[id][1] = (status?1:-1)*abs(mesh->components.intervals[id][1]);
        mesh->components.update();
    }
    con<RenderCtrl>().update();
}

template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}
