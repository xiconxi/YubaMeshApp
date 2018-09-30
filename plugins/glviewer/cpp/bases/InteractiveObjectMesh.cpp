#include "InteractiveObjectMesh.h"
#include <glm/gtc/type_ptr.hpp>
#include <QOpenGLFunctions_4_0_Core>
#include <easylogging++.h>
#include "../controller/CentralController.h"
#include "../controller/InteractiveController.h"
#include "../utils/Select.h"
#include <YbMesh/YbMesh.hpp>

void IModelTransform::rotate(int dx, int dy) {
    if (abs(dx) > abs(dy)) {
        model = glm::rotate(glm::mat4(), dx * 0.003f, glm::vec3(0, 1, 0)) * model;
    } else if (abs(dy) > abs(dx) ) {
        model = glm::rotate(glm::mat4(),-dy * 0.003f, glm::vec3(1, 0, 0)) * model;
    }
}

QMatrix4x4 IModelTransform::Model(){
    return QMatrix4x4(glm::value_ptr(this->model), 4, 4);
}

void IModelTransform::scaleBy(float s) {
    model = glm::scale(glm::mat4(), (1+s)*glm::vec3(1,1,1)) * model;
}

IDrawObject::IDrawObject(TriMesh&& vmesh, int _components):m_v(vmesh),components(_components, _components == 1? m_v.f().size():0),
            m_n(YbMesh::indicesTriMesh<glm::vec3>(std::make_shared<std::vector<glm::vec3>>(),m_v.shared_f())){
}

void IDrawObject::createBufferScript() {
    gl.glGenVertexArrays(1, &vao);
    gl.glGenBuffers(1, &v_buffer);
    gl.glGenBuffers(1, &ibo);
}

void IDrawObject::syncFacesBuffersDataScript() {
    auto& f = m_v.f();
    gl.glBindBuffer(GL_ARRAY_BUFFER, ibo);
    gl.glBufferData(GL_ARRAY_BUFFER, f.size()*sizeof(f[0]), f.data(), GL_STATIC_DRAW);
}

void IDrawObject::syncVertexBuffersDataScript() {
    auto& f = m_v.f();
    auto& v = m_v.v();
    auto& vn = m_n.v();
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, v_buffer);
    gl.glBufferData(GL_ARRAY_BUFFER, v.size()*(sizeof(v[0])+sizeof(vn[0])), nullptr, GL_STATIC_DRAW);
    gl.glBufferSubData(GL_ARRAY_BUFFER, 0, v.size()*sizeof(v[0]), v.data());
    gl.glBufferSubData(GL_ARRAY_BUFFER, v.size()*sizeof(v[0]), vn.size()*sizeof(vn[0]), vn.data());
    gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(v[0]), (GLvoid*)0);
    gl.glEnableVertexAttribArray(0);

    gl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vn[0]), (GLvoid*)(v.size()*sizeof(v[0])));
    gl.glEnableVertexAttribArray(1);

    gl.glBindVertexArray(0);
}

void IDrawObject::drawElementScript(uint start, uint size) {
    drawElementBufferScript(ibo,start,size);
}

void IDrawObject::_::update() {
    for(int i = 0; i < intervals.size(); i++) {
        counts[i] = (intervals[i][1] > 0?(intervals[i][1] - intervals[i][0]):0)*3;
        offset[i] = (const void*)(intervals[i][0]*sizeof(glm::vec3));
    }
}

void IDrawObject::multiDrawElementScript() {
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    gl.glMultiDrawElements(GL_TRIANGLES, components.counts.data(), GL_UNSIGNED_INT, components.offset.data(), components.counts.size());
    gl.glBindVertexArray(0);
}

void IDrawObject::drawElementBufferScript(uint buffer_id, uint start, uint size) {
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
    gl.glDrawElements(GL_TRIANGLES, (size?size:m_v.f().size())*3, GL_UNSIGNED_INT, (GLvoid*)(start*3*sizeof(unsigned int)));
    gl.glBindVertexArray(0);
}

void IDrawObject::calculateNorm() {
    YbMesh::visualization::calculateNorm(m_v, m_n);
}

void IDrawObject::normalize(bool centralized) {
    model = YbMesh::visualization::normalize(m_v,centralized);
}

IDrawObject::~IDrawObject(){
    gl.glDeleteVertexArrays(1, &vao);
    gl.glDeleteBuffers(1, &v_buffer);
    gl.glDeleteBuffers(1, &ibo);
}

InteractiveObject::~InteractiveObject() {
    gl.glDeleteBuffers(1, &selected_buffer);
}

InteractiveObject::InteractiveObject(TriMesh&& vmesh, int components):QObject(nullptr),IDrawObject(std::move(vmesh), components){

}

void InteractiveObject::createBufferScript() {
    IDrawObject::createBufferScript();
    gl.glGenBuffers(1, &selected_buffer);
}

void InteractiveObject::syncSelectBufferScript() {
    global::con<InteractiveCtrl>().selectTool->syncBufferScript(this);
}

void InteractiveObject::downloadSelectedBufferScript(int buffer_size) {
    selected_faces.resize(buffer_size);
    gl.glBindBuffer(GL_COPY_READ_BUFFER, selected_buffer);
    const void *data = gl.glMapBuffer(GL_COPY_READ_BUFFER, GL_READ_ONLY);
    if (data)
        memcpy(selected_faces.data(), data, selected_faces.size() * sizeof(selected_faces[0]));
    gl.glUnmapBuffer(GL_COPY_READ_BUFFER);
    gl.glBindBuffer(GL_COPY_READ_BUFFER, 0);
}

const std::vector<glm::ivec3>& InteractiveObject::selectedFaces() {
    return selected_faces;
}
