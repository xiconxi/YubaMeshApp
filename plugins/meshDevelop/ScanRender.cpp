#include "ScanRender.h"
#include <functional>
#include <easylogging++.h>
#include <YbCore/scripts>
#include "PluginBackend.h"

ScanRender::ScanRender():RenderScript(std::bind(&ScanRender::draw, this, std::placeholders::_1)){

}

void ScanRender::draw(QTime &t) {
    auto shader = con<ShaderCtrl>().shader("base");
    auto view = con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    for(auto objectKV:con<InteractiveCtrl>().allObjects()){
        auto mesh = objectKV.second;
        if(mesh->visible == false) continue;
        if(dynamic_cast<VertexClusterMeshObject*>(mesh) == nullptr) continue;
        shader->setUniformValue("model", view->Model()*mesh->Model());
        mesh->drawElementScript();
    }
    shader->release();
}
