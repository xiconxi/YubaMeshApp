#include "ScriptSamples.h"
#include <easylogging++.h>
#include "../controller/CentralController.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"

RenderFunc YbCore::defaultScript::test2 = [](QTime& t){
    LOG(INFO) << "test YbCore::test passed. ";
};

void YbCore::defaultScript::clear(QTime &t) {
    gl.glEnable(GL_DEPTH_TEST);
    gl.glClearColor(0.2, 0.3, 0.3, 1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void YbCore::defaultScript::test1(QTime &t) {
//    gl.glEnable(GL_DEPTH_TEST);
//    gl.glClearColor(0.2, 0.3, 0.3, 1.0);
//    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    auto shader = con<ShaderCtrl>().shader("core",true);
//    auto view = con<ViewCtrl>().view();
//    shader->bind();
//    for(auto& meshKV:con<MeshCtrl>().allMesh()) {
//        auto mesh = meshKV.second;
//        if(mesh->visible == false) continue;
//        if(con<MeshCtrl>().selectable() == mesh)
//            shader->setUniformValue("base_color", 0.0f,0.2f,0.0f);
//        else
//            shader->setUniformValue("base_color", 0.2f,0.2f,0.2f);
//        shader->setUniformValue("camera_vp", view->MatrixVP());
//        shader->setUniformValue("model", view->Model()*mesh->Model());
//        mesh->drawElementScript();
//    }
//    shader->release();
}
