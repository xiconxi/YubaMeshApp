#include "develop_backend.h"
#include <functional>
#include <easylog/easylogging++.h>
#include <glviewer/manager/ShaderManager.h>
#include <glviewer/manager/MeshManager.h>
#include <glviewer/manager/ViewManager.h>
#include <glviewer/mesh/meshCodes.h>
#include <glviewer/mesh/meshProcessing.h>
#include "algorithm/half_edge.h"
#include "algorithm/meshProcessing.h"

#include <QStandardPaths>
#include <fstream>
// OPENCV

INITIALIZE_EASYLOGGINGPP
develop_backend::develop_backend(QObject* parent)
    : QObject(parent)
{

}
glm::vec3 orientN;

A::A():RenderScript(std::bind(&A::scan_line_animation,this,std::placeholders::_1)){
    RenderScript([this](RenderScript* f){
        auto& mesh = *(TMM("bunny")->Mesh());
        orientN = glm::normalize(glm::mat3(glm::inverse(mesh.model))*glm::vec3(0,1,0));
        this->dot_v = glm::vec2(glm::dot(orientN, mesh.v[(*mesh.f.begin())[0]].mv), glm::dot(orientN, mesh.v[(*mesh.f.end())[0]].mv));
        auto he = halfEdge(&mesh, mesh.f.begin(), mesh.f.end());

        std::fstream fileHandle;
        fileHandle.open(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/a.obj",std::ifstream::out);
//        for(float percent = 0.1; percent < 0.9; percent += 0.02)
        float percent = 1-0.72;
        for(auto line:he.get_slice(orientN, this->dot_v[0]+(this->dot_v[1]-this->dot_v[0])*percent,(this->dot_v[1]-this->dot_v[0])*0.05)) {
            fileHandle << "v " << line[0][0] << ' ' << line[0][1] << ' ' << line[0][2] << std::endl;
            fileHandle << "v " << line[1][0] << ' ' << line[1][1] << ' ' << line[1][2] << std::endl;
        }
        fileHandle.close();
    });
}

void A::scan_line_animation(RenderScript* f) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = SMM("base");
    auto mesh = TMM("bunny");

    if(mesh->texture)
        mesh->texture->bind();

    shader->bind();
    shader->setUniformValue("camera_vp", VMM("cam")->MatrixVP());
    shader->setUniformValue("model", VMM("cam")->Model()*mesh->Model());
    auto v = glm::vec4(orientN,dot_v[0]+(dot_v[1]-dot_v[0])*percent);
    shader->setUniformValue("orient", v[0],v[1],v[2],v[3]);
    auto glmesh = mesh->Mesh();
    auto p = std::lower_bound(glmesh->f.begin(),glmesh->f.end(), dot_v[0]+(dot_v[1]-dot_v[0])*percent,
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(glmesh->v[e1[0]].mv, orientN);
    });

    mesh->indicesDraw(0, (p - glmesh->f.begin()));
    shader->release();

    percent += 1e-3;
    percent = percent > 1.0 ? 0: percent;
    TR->update(true);
}

void A::draw_model(RenderScript *f) {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = SMM("base");
    auto mesh = TMM("bunny");

    if(mesh->texture)
        mesh->texture->bind();

    shader->bind();
    shader->setUniformValue("camera_vp", VMM("cam")->MatrixVP());
    shader->setUniformValue("model", VMM("cam")->Model()*mesh->Model());
    auto glmesh = mesh->Mesh();
    auto p1 = std::lower_bound(glmesh->f.begin(),glmesh->f.end(), dot_v[0]+(dot_v[1]-dot_v[0])*(percent-0.01),
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(glmesh->v[e1[0]].mv, glm::mat3(glm::inverse(glmesh->model))*glm::vec3(0,1,0));
    });

    auto p2 = std::lower_bound(glmesh->f.begin(),glmesh->f.end(), dot_v[0]+(dot_v[1]-dot_v[0])*(percent+0.01),
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(glmesh->v[e1[0]].mv, glm::mat3(glm::inverse(glmesh->model))*glm::vec3(0,1,0));
    });

    mesh->indicesDraw( (p1 - glmesh->f.begin()), (p2 - p1));

    shader->release();
}

void develop_backend::initial_gl_resource() {
    render_a = new("Class_Render") A;
}

void develop_backend::draw_for(QString value) {
    if(value == "mesh") {
        render_a->set_render(std::bind(&A::draw_model,render_a,std::placeholders::_1));
    }
}

void develop_backend::glinit() {
    static bool is_initialed = false;
    if(!is_initialed && (is_initialed=true))
        this->initial_gl_resource();
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
