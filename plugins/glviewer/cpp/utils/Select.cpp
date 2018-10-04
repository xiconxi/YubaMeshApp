#include "Select.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/InteractiveController.h"
#include <easylogging++.h>
#include <QSGTextureProvider>
#include "../bases/InteractiveObjectMesh.h"
SelectTool::SelectTool():RenderScript(std::bind(&SelectTool::drawResultSrcipt, this, std::placeholders::_1))
{

}

void SelectTool::clearSelect() {
    stream_size[0] = 0;
}

void SelectTool::SelectScript(QTime& t) {
    auto shader = global::con<ShaderCtrl>().shader("select");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    for(auto objectKV:global::con<InteractiveCtrl>().allObjects()){
        auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
        if(mesh == nullptr || mesh->visible == false) continue;
        this->beginStreamQueryScript();

        shader->setUniformValue("model", view->Model()*mesh->Model());
        gl.glBindTexture(GL_TEXTURE_2D, areaItem->textureProvider()->texture()->textureId());

        gl.glEnable(GL_RASTERIZER_DISCARD);
        gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
        gl.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,mesh->selected_buffer);
        gl.glBeginTransformFeedback(GL_POINTS);

        mesh->drawElementScript();

        gl.glDisable(GL_RASTERIZER_DISCARD);
        gl.glEndTransformFeedback();
        gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        this->endStreamQueryScript();
        mesh->downloadSelectedBufferScript(stream_size[0]);
        if(stream_size[0] > 0)
            emit mesh->FaceSelected(mesh);
    }
    for(auto objectKV:plugin::con<InteractiveCtrl>().allObjects()){
        auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
        if(mesh == nullptr || mesh->visible == false) continue;
        this->beginStreamQueryScript();

        shader->setUniformValue("model", view->Model()*mesh->Model());
        gl.glBindTexture(GL_TEXTURE_2D, areaItem->textureProvider()->texture()->textureId());

        gl.glEnable(GL_RASTERIZER_DISCARD);
        gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, xfb);
        gl.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,mesh->selected_buffer);
        gl.glBeginTransformFeedback(GL_POINTS);

        mesh->drawElementScript();

        gl.glDisable(GL_RASTERIZER_DISCARD);
        gl.glEndTransformFeedback();
        gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        this->endStreamQueryScript();
        mesh->downloadSelectedBufferScript(stream_size[0]);

        if(stream_size[0] > 0)
            emit mesh->FaceSelected(mesh);
    }
    shader->release();
}

void SelectTool::drawResultSrcipt(QTime& t) {
    auto shader = global::con<ShaderCtrl>().shader("selection");
    auto view = global::con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", global::con<ViewCtrl>().view()->MatrixVP());
    gl.glEnable(GL_POLYGON_OFFSET_FILL);
    gl.glPolygonOffset(-1.0,-1.0);
    for(auto objectKV:global::con<InteractiveCtrl>().allObjects()){
        auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
        if(mesh == nullptr || mesh->selected_faces.size() == 0) continue;
        shader->setUniformValue("model", global::con<ViewCtrl>().view()->Model()*mesh->Model());
        mesh->drawElementBufferScript(mesh->selected_buffer,0,mesh->selected_faces.size());
    }
    for(auto objectKV:plugin::con<InteractiveCtrl>().allObjects()){
        auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
        if(mesh == nullptr || mesh->selected_faces.size() == 0) continue;
        shader->setUniformValue("model", global::con<ViewCtrl>().view()->Model()*mesh->Model());
        mesh->drawElementBufferScript(mesh->selected_buffer,0,mesh->selected_faces.size());
    }
    shader->release();
    gl.glDisable(GL_POLYGON_OFFSET_FILL);
}

void SelectTool::createBufferScript() {
    gl.glGenTransformFeedbacks(1, &xfb);
    gl.glGenVertexArrays(1, &face_vao);
}

void SelectTool::syncBufferScript(InteractiveObject* object) {
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->xfb);
    gl.glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, object->selected_buffer);
    gl.glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, object->m_v.f().size() * sizeof(int) * 3, nullptr, GL_DYNAMIC_COPY);
    gl.glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0,object->selected_buffer);
    gl.glBindVertexArray( this->face_vao);
    gl.glBindBuffer(GL_ARRAY_BUFFER, object->selected_buffer);
    gl.glVertexAttribIPointer(0, 3, GL_INT, sizeof(GLint)*3, NULL);
    gl.glEnableVertexAttribArray(0);
    gl.glBindVertexArray(0);
    gl.glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
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
