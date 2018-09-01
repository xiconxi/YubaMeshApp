#include "ScanRender.h"
#include <functional>
#include <easylogging++.h>
#include <YbCore/scripts>

ScanRender::ScanRender():RenderScript(std::bind(&ScanRender::draw, this, std::placeholders::_1)){

}

void ScanRender::draw(QTime &t) {
    gl.glEnable(GL_DEPTH_TEST);
    gl.glClearColor(0.2, 0.3, 0.3, 1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = con<ShaderCtrl>().shader("base");
    auto view = con<ViewCtrl>().view();
    auto mesh = con<InteractiveCtrl>().object("scanbody");
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*mesh->Model());
    mesh->drawElementScript();
    shader->release();
}
