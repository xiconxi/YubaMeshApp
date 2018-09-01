#include "Select.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/InteractiveController.h"
#include <easylogging++.h>
#include <QSGTextureProvider>
#include "../mesh/PickableMesh.h"

SelectTool::SelectTool():RenderScript(std::bind(&SelectTool::drawResultSrcipt, this, std::placeholders::_1))
{

}

void SelectTool::clearSelect() {
    stream_size[0] = 0;
}

void SelectTool::SelectScript(QTime& t) {
    auto shader = con<ShaderCtrl>().shader("select",true);
    auto view = con<ViewCtrl>().view();
    auto mesh = con<InteractiveCtrl>().focus();
    if(mesh == nullptr) return ;
    this->beginStreamQueryScript();

    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*mesh->Model());
    gl.glBindTexture(GL_TEXTURE_2D, areaItem->textureProvider()->texture()->textureId());

    gl.glEnable(GL_RASTERIZER_DISCARD);
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
    gl.glBeginTransformFeedback(GL_POINTS);

    mesh->drawElementScript();

    gl.glDisable(GL_RASTERIZER_DISCARD);
    gl.glEndTransformFeedback();
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

    shader->release();

    this->endStreamQueryScript();
    if(stream_size[0] > 0) {

//        this->downloadSelectionsScript();
//        emit mesh->FaceSelected(mesh);
    }
}

void SelectTool::downloadSelectionsScript() {
    selected_faces.resize(stream_size[0]);
    gl.glBindBuffer(GL_COPY_READ_BUFFER, face_buffer);
    const void *data = gl.glMapBuffer(GL_COPY_READ_BUFFER, GL_READ_ONLY);
    if (data)
        memcpy(selected_faces.data(), data, selected_faces.size() * sizeof(selected_faces[0]));
    gl.glUnmapBuffer(GL_COPY_READ_BUFFER);
    gl.glBindBuffer(GL_COPY_READ_BUFFER, 0);
}

std::vector<glm::ivec3>& SelectTool::getSelectedFace() {
    return selected_faces;
}

void SelectTool::drawResultSrcipt(QTime& t) {
    if(stream_size[0] > 0 ) {
        gl.glEnable(GL_POLYGON_OFFSET_FILL);
        gl.glPolygonOffset(-1.0,-1.0);
        auto shader = con<ShaderCtrl>().shader("selection",true);
        auto mesh = con<InteractiveCtrl>().focus();
        shader->bind();
        shader->setUniformValue("camera_vp", con<ViewCtrl>().view()->MatrixVP());
        shader->setUniformValue("model", con<ViewCtrl>().view()->Model()*mesh->Model());
        mesh->drawElementBufferScript(face_buffer, 0, stream_size[0]);
        shader->release();
        gl.glDisable(GL_POLYGON_OFFSET_FILL);
    }
}

void SelectTool::sizingBufferScriptWrap(GLuint f_size) {
    if(f_size > current_face_buffer_size) {
        current_face_buffer_size = f_size;
        AnonymouseScript(SelectTool::syncBufferScript);
    }
}

void SelectTool::createBufferScript() {
    gl.glGenTransformFeedbacks(1, &xfb);
    gl.glGenBuffers(1, &face_buffer);
    gl.glGenVertexArrays(1, &face_vao);
}

void SelectTool::syncBufferScript(QTime &t) {
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->xfb);
    gl.glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, this->face_buffer);
    gl.glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, current_face_buffer_size * sizeof(int) * 3, nullptr, GL_DYNAMIC_COPY);
    gl.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,this->face_buffer);
    gl.glBindVertexArray( this->face_vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, this->face_buffer);
    gl.glVertexAttribIPointer(0, 3, GL_INT, sizeof(GLint)*3, NULL);
    gl.glEnableVertexAttribArray(0);
}


void SelectTool::areasFaceSelect(QQuickPaintedItem *areaItem) {
    this->areaItem = areaItem;
    AnonymouseScript(SelectTool::SelectScript);
}

void SelectTool::beginStreamQueryScript() {
    if (queries[0] == 0)
        gl.glGenQueries(2, &queries[0]);
    gl.glBeginQueryIndexed(GL_PRIMITIVES_GENERATED, 0, queries[0]);
    gl.glBeginQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, 0, queries[1]);
}

void SelectTool::endStreamQueryScript() {
    gl.glEndQueryIndexed(GL_PRIMITIVES_GENERATED, 0);
    gl.glEndQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, 0);
    gl.glGetQueryObjectiv(queries[0], GL_QUERY_RESULT, &stream_size[0]);
    gl.glGetQueryObjectiv(queries[1], GL_QUERY_RESULT, &stream_size[1]);
//    LOG(INFO) << "stream size: " << stream_size[0] << ' ' << stream_size[1];
    //        if (stream_size[i]) std::cout << " stream[Clips]: " << i << ' ' << stream_size[i] << std::endl;
}
