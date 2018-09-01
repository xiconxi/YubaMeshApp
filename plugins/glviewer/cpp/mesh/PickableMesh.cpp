#include "PickableMesh.h"
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

void IDrawObject::drawElementBufferScript(uint buffer_id, uint start, uint size) {
    gl.glBindVertexArray(vao);
    gl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
    gl.glDrawElements(GL_TRIANGLES, (size?size:m_v.f().size())*3, GL_UNSIGNED_INT, (GLvoid*)(start*3*sizeof(unsigned int)));
    gl.glBindVertexArray(0);
}

void IDrawObject::calculateNorm() {
    YbMesh::visualization::calculateNorm(m_v, m_n);
}

void IDrawObject::centerlized() {
    model = YbMesh::visualization::centerlized(m_v);
}

InteractiveObject::InteractiveObject(TriMesh vmesh,TriMesh nmesh):IDrawObject(vmesh,nmesh),QObject(nullptr){

}

void InteractiveObject::createBufferScript() {
    IDrawObject::createBufferScript();
    gl.glGenBuffers(1, &selected_buffer);
}

void InteractiveObject::syncSelectBufferScript() {
    con<InteractiveCtrl>().selectTool->syncBufferScript(this);
}

InteractiveObject::~InteractiveObject(){

}
