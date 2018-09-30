#include "SliceRender.h"
#include <YbCore/controller>
#include "PluginBackend.h"

SliceRender::SliceRender():RenderScript(std::bind(&SliceRender::drawModelWithSlice,this,std::placeholders::_1))
{

}

void SliceRender::updateDis(float value) {

}

void SliceRender::drawModelWithSlice(QTime &t) {
    auto mesh = plugin::con<InteractiveCtrl>().focus();
    auto shader = plugin::con<ShaderCtrl>().shader("base");
    auto view = global::con<ViewCtrl>().view();
    if(mesh == nullptr || shader == nullptr) return;
    shader->bind();
    auto norm = static_cast<PluginBackend*>(Backend<PluginBackend>())->norm;
    shader->setUniformValue("orient", norm[0],norm[1],norm[2],dis);
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("base_color", 0.2f,0.2f,0.2f);
    shader->setUniformValue("model", view->Model()*mesh->Model());
    mesh->drawElementScript();
    shader->release();
}

void SliceRender::scanLineAnimation(QTime &t) {
    gl.glEnable(GL_DEPTH_TEST);
    gl.glClearColor(0.2, 0.3, 0.3, 1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto mesh = global::con<InteractiveCtrl>().focus();
    auto shader = plugin::con<ShaderCtrl>().shader("base");
    auto view = global::con<ViewCtrl>().view();
    if(mesh == nullptr || shader == nullptr) return;
    shader->bind();
    shader->setUniformValue("base_color", 0.2f,0.2f,0.2f);
    auto norm = static_cast<PluginBackend*>(Backend<PluginBackend>())->norm;
    shader->setUniformValue("orient", norm[0],norm[1],norm[2],glm::mix(bounding_z[0],bounding_z[1],percent));
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*mesh->Model());
    mesh->drawElementScript();
    percent += percent < 1.0 ? 1e-3:-percent;
    static_cast<PluginBackend*>(Backend<PluginBackend>())->slice(glm::mix(bounding_z[0],bounding_z[1],percent));
    shader->release();
    global::con<RenderCtrl>().update(true);
}
