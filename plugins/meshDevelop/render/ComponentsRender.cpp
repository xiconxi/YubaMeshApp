#include "ComponentsRender.h"
#include <functional>
#include <easylogging++.h>
#include <YbCore/scripts>
#include "PluginBackend.h"

ComponentsRender::ComponentsRender(TriMesh&& vmesh,int components):
    IDrawObject(std::move(vmesh),components),
    vertex_cluster(vmesh.v().size(),0),
    RenderScript(std::bind(&ComponentsRender::draw, this, std::placeholders::_1))
{
    this->normalize();
    this->calculateNorm();
    componetAnalysic();

    RenderScript([this](QTime&) {
        this->createBufferScript();
        this->syncVertexBuffersDataScript();
        this->syncFacesBuffersDataScript();
    });
}

void ComponentsRender::draw(QTime &t) {
    if(this->visible == false) return;
    auto shader = plugin::con<ShaderCtrl>().shader("base");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*this->Model());
    this->multiDrawElementScript();
    shader->release();
}

void ComponentsRender::componetAnalysic(){
    auto& triMesh = this->m_v;
    glm::mat3 pca = YbMesh::slice::pca_analysic(triMesh.v(), triMesh.f().begin(), triMesh.f().end());
    YbMesh::slice::sortByVector(triMesh, pca[2]);

    std::array<float,2> Z = {glm::dot(pca[2],triMesh.v()[(*std::prev(triMesh.f().end())).r]),glm::dot(pca[2],triMesh.v()[(*triMesh.f().begin()).r])};
    std::vector<float> z_dots(triMesh.v().size());
    for(int i = 0; i < triMesh.v().size(); i++)
        z_dots[i] = glm::dot(triMesh.v()[i], pca[2]);

    double mean_dis = std::sqrt(std::accumulate(triMesh.v().begin(),triMesh.v().end(), 0.0, [=](double acc, glm::vec3& e){
                        return acc+std::powf(glm::length(e-pca[2]*glm::dot(e, pca[2]) ),2);})/triMesh.v().size())*0.9f;

    std::vector<std::array<float,2>> ratio_intervals = {
        {glm::mix(Z[0],Z[1],0.00), glm::mix(Z[0],Z[1],0.42)}, // 腿
        {glm::mix(Z[0],Z[1],0.42), glm::mix(Z[0],Z[1],0.54)},
        {glm::mix(Z[0],Z[1],0.54), glm::mix(Z[0],Z[1],0.74)}, // 躯干
        {glm::mix(Z[0],Z[1],0.74), glm::mix(Z[0],Z[1],0.84)}, // 肩膀
        {glm::mix(Z[0],Z[1],0.84), glm::mix(Z[0],Z[1],1.00)}, // 头部
    };

    for(int i = 0; i < triMesh.v().size(); i++) {
        if(z_dots[i] >= (*ratio_intervals.begin())[0] && z_dots[i] <= (*ratio_intervals.begin())[1]) {
            this->vertex_cluster[i] = glm::dot(triMesh.v()[i], pca[0]) < 0 ? 2:1;
        }else {
            if( glm::length(triMesh.v()[i]-pca[2]*z_dots[i] ) > mean_dis ){
                this->vertex_cluster[i] = glm::dot(triMesh.v()[i], pca[0]) < 0 ? 4:3;
            } else for(auto it = ratio_intervals.begin() + 1; it != ratio_intervals.end(); it++) {
                if(z_dots[i] >= (*it)[0] && z_dots[i] <= (*it)[1]) {
                    this->vertex_cluster[i] = 4 + it - ratio_intervals.begin() ;
                }
            }
        }
    }

    std::sort(triMesh.f().begin(), triMesh.f().end(), [z_dots,this](const glm::vec3& e1, const glm::vec3& e2){
        return this->vertex_cluster[e1.r] == this->vertex_cluster[e2.r] ?
               z_dots[e1.r] < z_dots[e2.r]:this->vertex_cluster[e1.r] < this->vertex_cluster[e2.r];
    });
    auto& intervals = this->components.intervals;
    for(int i = 0, offset = 0; i < 8; i++) {
        intervals[i][0] = offset;
        intervals[i][1] = std::lower_bound(triMesh.f().begin(), triMesh.f().end(), i+1 ,[this](const glm::vec3& e1, const int v){
                                             return this->vertex_cluster[e1.r] <= v; }) - triMesh.f().begin();
        offset = intervals[i][1];
    }
    this->components.update();
    auto box = global::con<InteractiveCtrl>().object("box");
    box->normalize();
    box->model = this->model*YbMesh::visualization::boundingBox(triMesh,pca)*box->model;

    this->model = glm::rotate(glm::mat4(),3.1415926f,glm::vec3(0,0,1))*glm::mat4(glm::transpose(pca)) * this->model;
}

void ComponentsRender::syncVertexBuffersDataScript() {
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
