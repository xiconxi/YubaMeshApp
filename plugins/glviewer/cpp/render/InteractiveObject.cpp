#include "InteractiveObject.h"
#include "../controller/CentralController.h"
#include "../controller/GLMeshController.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../utils/Select.h"
#include <QSGTextureProvider>

///
/// \brief The face_feedback_shared_vao struct
/// 同一类面元结构共享
struct face_feedback_shared_vao{
    uint face_vao;
    face_feedback_shared_vao(){
//        RenderScript([this](QTime &t){
        gl.glGenVertexArrays(1, &face_vao);
        gl.glBindVertexArray(face_vao);
        gl.glVertexAttribIPointer(0, 3, GL_INT, sizeof(GLint)*3, NULL);
        gl.glEnableVertexAttribArray(0);
        gl.glBindVertexArray(0);
//        });
    }
};

template<class T>
T& con(){
    return ICtrl<T>::getGrobalInstanceRef();
}

InteractiveObject::~InteractiveObject() {
    gl.glDeleteBuffers(1, &selected_buffer);
}

InteractiveObject::InteractiveObject(TriMesh&& vmesh, int components):QObject(nullptr),IGLMeshObject(std::move(vmesh), components){

}

void InteractiveObject::createBufferScript() {
    IGLMeshObject::createBufferScript();
    gl.glGenBuffers(1, &selected_buffer);
}

void InteractiveObject::syncSelectBufferScript() {
    auto selectTool = global::con<GLMeshCtrl>().selectTool;
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, selectTool->xfb);
    gl.glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, selected_buffer);
    gl.glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, m_v.f().size() * sizeof(int) * 3, nullptr, GL_DYNAMIC_COPY);
    gl.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, selected_buffer);
    gl.glBindVertexArray(con<face_feedback_shared_vao>().face_vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, this->selected_buffer);
    gl.glBindVertexArray(0);
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
}

void InteractiveObject::selectScript(QTime& t) {
    auto view = global::con<ViewCtrl>().view();
    auto shader = global::con<ShaderCtrl>().shader("select");
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

void InteractiveObject::selectionVisualizedScript(QTime &t) {
    auto shader = global::con<ShaderCtrl>().shader("selection");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", global::con<ViewCtrl>().view()->MatrixVP());
    shader->setUniformValue("model", global::con<ViewCtrl>().view()->Model()*this->Model());
    InteractiveObject::drawElementBufferScript(this->selected_buffer,0,this->selected_faces.size());
    shader->release();
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
