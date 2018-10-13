#include "InteractiveFaceTexObject.h"
#include "../controller/CentralController.h"
#include "../controller/GLMeshController.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../utils/Select.h"
#include <QSGTextureProvider>


InteractiveFaceTexObject::InteractiveFaceTexObject(TriMesh &mesh, YbMesh::indicesTriMesh<glm::vec2> &texMesh)
    :InteractiveObject(std::move(mesh)),texM(std::move(texMesh)) {

}

void InteractiveFaceTexObject::syncVertexBuffersDataScript() {
    auto& v = m_v.v();
    auto& vn = m_n.v();
    std::vector<glm::vec3> tex(v.size());
    for(int i = 0; i < tex.size(); i++) tex[i] = glm::vec3(texM.v()[i],1.0f);
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    gl.glBufferData(GL_ARRAY_BUFFER, v.size()*(sizeof(v[0])+sizeof(vn[0])+sizeof(tex[0])), nullptr, GL_STATIC_DRAW);

    gl.glBufferSubData(GL_ARRAY_BUFFER, 0, v.size()*sizeof(v[0]), v.data());
    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), v.size()*sizeof(tex[0]), tex.data());
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v[0]), (GLvoid*)0);
    gl.glEnableVertexAttribArray(0);

    gl.glBufferSubData(GL_ARRAY_BUFFER, 2*v.size()*sizeof(v[0]), vn.size()*sizeof(vn[0]), vn.data());
    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vn[0]), (GLvoid*)(2*v.size()*sizeof(v[0])));
    gl.glEnableVertexAttribArray(1);

    gl.glBindVertexArray(0);
}

void InteractiveFaceTexObject::drawElementBufferScript(uint buffer_id, uint start, uint size) {
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
    gl.glDrawElements(GL_TRIANGLES_ADJACENCY, (size?size:m_v.f().size())*6, GL_UNSIGNED_INT, (GLvoid*)(start*6*sizeof(unsigned int)));
    gl.glBindVertexArray(0);
}

void InteractiveFaceTexObject::syncFacesBuffersDataScript() {
    auto& f1 = m_v.f();
    auto& f2 = texM.f();
    int offset = m_v.v().size();
    std::vector< std::array<int,6> > tri_adjacency(m_v.f().size());
    for(int i = 0; i < tri_adjacency.size(); i++) {
        tri_adjacency[i] = {f1[i][0],f2[i][0]+offset,f1[i][1],f2[i][1]+offset,f1[i][2],f2[i][2]+offset};
    }

    gl.glBindBuffer(GL_ARRAY_BUFFER, ibo);
    gl.glBufferData(GL_ARRAY_BUFFER, tri_adjacency.size()*sizeof(tri_adjacency[0]), tri_adjacency.data(), GL_STATIC_DRAW);
}

void InteractiveFaceTexObject::selectScript(QTime& t) {
    auto view = global::con<ViewCtrl>().view();
    auto shader = global::con<ShaderCtrl>().shader("select_adjacency");
    auto selectTool = global::con<GLMeshCtrl>().selectTool;
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    selectTool->beginStreamQueryScript();

    shader->setUniformValue("model", view->Model()*this->Model());
    gl.glBindTexture(GL_TEXTURE_2D, selectTool->areaItem->textureProvider()->texture()->textureId());

    gl.glEnable(GL_RASTERIZER_DISCARD);
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, selectTool->xfb);
    gl.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,this->selected_buffer);
    gl.glBeginTransformFeedback(GL_POINTS);

    this->drawElementScript();

    gl.glDisable(GL_RASTERIZER_DISCARD);
    gl.glEndTransformFeedback();
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    selectTool->endStreamQueryScript();
    this->downloadSelectedBufferScript(selectTool->stream_size[0]);
    if(selectTool->stream_size[0] > 0)
        emit this->FaceSelected(this);
    shader->release();
}

