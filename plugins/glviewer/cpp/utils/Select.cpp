#include "Select.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/GLMeshController.h"
#include <easylogging++.h>
#include <QSGTextureProvider>
#include <QOpenGLShaderProgram>
#include "../bases/GLMeshObject.h"
#include "../render/InteractiveObject.h"

SelectTool::SelectTool():RenderScript(std::bind(&SelectTool::drawResultSrcipt, this, std::placeholders::_1))
{

}

void SelectTool::clearSelect() {
    stream_size[0] = 0;
}

void SelectTool::drawResultSrcipt(QTime& t) {
    gl.glEnable(GL_POLYGON_OFFSET_FILL);
    gl.glPolygonOffset(-1.0,-1.0);
    for(auto objectKV:global::con<GLMeshCtrl>().allObjects()){
        auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
        if(mesh == nullptr || mesh->selected_faces.size() == 0) continue;
        mesh->selectionVisualizedScript(t);
    }
    for(auto objectKV:plugin::con<GLMeshCtrl>().allObjects()){
        auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
        if(mesh == nullptr || mesh->selected_faces.size() == 0) continue;
        mesh->selectionVisualizedScript(t);
    }
    gl.glDisable(GL_POLYGON_OFFSET_FILL);
}

void SelectTool::createBufferScript() {
    gl.glGenTransformFeedbacks(1, &xfb);
}

void SelectTool::areasFaceSelect(QQuickPaintedItem *areaItem) {
    this->areaItem = areaItem;
    RenderScript([this](QTime& t){
        for(auto objectKV:global::con<GLMeshCtrl>().allObjects()){
            auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
            if(mesh == nullptr || mesh->visible == false) continue;
            mesh->selectScript(t);
        }
        for(auto objectKV:plugin::con<GLMeshCtrl>().allObjects()){
            auto mesh = dynamic_cast<InteractiveObject*>(objectKV.second);
            if(mesh == nullptr || mesh->visible == false) continue;
            mesh->selectScript(t);
        }

    });
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
