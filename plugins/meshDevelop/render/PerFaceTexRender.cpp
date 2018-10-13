#include "PerFaceTexRender.h"
#include <YbCore/scripts>

PerFaceTexRender::PerFaceTexRender(TriMesh& mesh,YbMesh::indicesTriMesh<glm::vec2>& texMesh)
    :InteractiveFaceTexObject(mesh,texMesh),
    RenderScript(YbCore::defaultScript::nullRender)
{
    this->normalize();
    this->calculateNorm();
    RenderScript([this](QTime&) {
        this->texture = new  QOpenGLTexture(QImage("/Users/hotpot/data/007/007Image1.png").mirrored());
        this->texture->bind();
        this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
        this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
        this->texture->setWrapMode(QOpenGLTexture::Repeat);
        this->texture->release();

        this->createBufferScript();
        this->syncVertexBuffersDataScript();
        this->syncFacesBuffersDataScript();
        this->syncSelectBufferScript();
    });
    setRender(std::bind(&PerFaceTexRender::draw, this, std::placeholders::_1));
}

//void PerFaceTexRender::syncVertexBuffersDataScript() {
//    auto& v = m_v.v();
//    auto& vn = m_n.v();
//    std::vector<glm::vec3> tex(v.size());
//    for(int i = 0; i < tex.size(); i++) tex[i] = glm::vec3(texM.v()[i],1.0f);
//    gl.glBindVertexArray(vao);
//    gl.glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
//    gl.glBufferData(GL_ARRAY_BUFFER, v.size()*(sizeof(v[0])+sizeof(vn[0])+sizeof(tex[0])), nullptr, GL_STATIC_DRAW);

//    gl.glBufferSubData(GL_ARRAY_BUFFER, 0, v.size()*sizeof(v[0]), v.data());
//    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), v.size()*sizeof(tex[0]), tex.data());
//    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v[0]), (GLvoid*)0);
//    gl.glEnableVertexAttribArray(0);

//    gl.glBufferSubData(GL_ARRAY_BUFFER, 2*v.size()*sizeof(v[0]), vn.size()*sizeof(vn[0]), vn.data());
//    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vn[0]), (GLvoid*)(2*v.size()*sizeof(v[0])));
//    gl.glEnableVertexAttribArray(1);

//    gl.glBindVertexArray(0);
//}

//void PerFaceTexRender::drawElementBufferScript(uint buffer_id, uint start, uint size) {
//    gl.glBindVertexArray(vao);
//    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
//    gl.glDrawElements(GL_TRIANGLES_ADJACENCY, (size?size:m_v.f().size())*6, GL_UNSIGNED_INT, (GLvoid*)(start*3*sizeof(unsigned int)));
//    gl.glBindVertexArray(0);
//}

//void PerFaceTexRender::syncFacesBuffersDataScript() {
//    auto& f1 = m_v.f();
//    auto& f2 = texM.f();
//    int offset = m_v.v().size();
//    std::vector< std::array<int,6> > tri_adjacency(m_v.f().size());
//    for(int i = 0; i < tri_adjacency.size(); i++) {
//        tri_adjacency[i] = {f1[i][0],f2[i][0]+offset,f1[i][1],f2[i][1]+offset,f1[i][2],f2[i][2]+offset};
//    }

//    gl.glBindBuffer(GL_ARRAY_BUFFER, ibo);
//    gl.glBufferData(GL_ARRAY_BUFFER, tri_adjacency.size()*sizeof(tri_adjacency[0]), tri_adjacency.data(), GL_STATIC_DRAW);
//}

void PerFaceTexRender::draw(QTime &t) {
    if(this->visible == false) return;
    auto shader = plugin::con<ShaderCtrl>().shader("adjacency_tex");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("vertex_size", (int)m_v.v().size());
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*this->Model());
    shader->setUniformValue("base_color", 0.2,0.2f, 0.2f);
    this->texture->bind();
    drawElementScript();
    this->texture->release();
    shader->release();
//    global::con<RenderCtrl>().update(true);
}
