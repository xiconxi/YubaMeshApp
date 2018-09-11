#include "PluginBackend.h"
#include "ScanRender.h"
#include <QStandardPaths>

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

VertexClusterMeshObject::VertexClusterMeshObject(TriMesh vmesh,TriMesh nmesh)
    :IDrawObject(vmesh,nmesh),vertex_cluster(vmesh.v().size(),0){
//    int strip = vertex_cluster.size()/12;
//    for(int i = 0; i < 12; i++)
//        std::fill(vertex_cluster.begin() + i*strip, vertex_cluster.begin() + (i+1)*strip, i);
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
    auto object = con<InteractiveCtrl>().interactiveObject("body2");
    if(object == nullptr) return;
    QObject::connect(object,&InteractiveObject::FaceSelected, object, [=](InteractiveObject* mesh){
        auto select_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/body2_selections.obj";
        LOG(INFO) << select_file;
        YbMesh::IO::writePartialMesh(mesh->m_v, mesh->selectedFaces(), select_file);
    });
}

void PluginBackend::draw_for(QString value) {

}

bool PluginBackend::importMesh(std::string url,std::string name){
    YbMesh::indicesTriMesh<glm::vec3> triMesh = YbMesh::IO::importOBJ_V0(url);
    auto object = new VertexClusterMeshObject(triMesh,YbMesh::indicesTriMesh<glm::vec3>(std::make_shared<std::vector<glm::vec3>>(),triMesh.f()));
    object->centerlized();
    object->calculateNorm();
    glm::mat3 pca = YbMesh::slice::pca_analysic(triMesh.v(), triMesh.f().begin(), triMesh.f().end());
    YbMesh::slice::sortByVector(triMesh, pca[2]);
    std::array<float,2> Z = {glm::dot(pca[2],triMesh.v()[(*std::prev(triMesh.f().end())).r]),glm::dot(pca[2],triMesh.v()[(*triMesh.f().begin()).r])};


    std::vector<std::array<float,2>> intervals = {
        {glm::mix(Z[0],Z[1],0.84),glm::mix(Z[0],Z[1],1.00)},
        {glm::mix(Z[0],Z[1],0.74),glm::mix(Z[0],Z[1],0.84)},
        {glm::mix(Z[0],Z[1],0.42),glm::mix(Z[0],Z[1],0.74)},
        {glm::mix(Z[0],Z[1],0.00),glm::mix(Z[0],Z[1],0.42)},
    };
    for(auto it = intervals.begin(); it != intervals.end(); it++) {
        for(int i = 0; i < triMesh.v().size(); i++) {
            float v = glm::dot(triMesh.v()[i], pca[2]);
            if(v >= (*it)[0] && v <= (*it)[1])
                object->vertex_cluster[i] = it - intervals.begin() ;
        }
    }
    for(int i = 0; i < triMesh.v().size(); i++) {
        if(object->vertex_cluster[i] == 3) {
            if(glm::dot(triMesh.v()[i], pca[1]) > 0)
                object->vertex_cluster[i]++ ;
        }

    }
    glm::mat4 coord = glm::rotate(glm::mat4(),-3.1415926f*0.5f,glm::vec3(0,0,1))* glm::mat4(glm::transpose(pca));
    object->model = coord * object->model;
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    con<InteractiveCtrl>().addInteractiveObject(name, object);
    return true;
}

template <>
QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    return ICtrl<PluginBackend>::getInstance();
}
