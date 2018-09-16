#include "auxiliary.h"
#include <YbMesh/YbMesh.hpp>
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/InteractiveController.h"
#include "../controller/RenderController.h"
#include "../bases/InteractiveObjectMesh.h"
void YbCore::aux::addCoord3d(float h, float r, std::string name) {
    auto triMesh = YbMesh::geometry::make_axes(h,r);
    auto object = new IDrawObject(triMesh,YbMesh::indicesTriMesh<glm::vec3>(std::make_shared<std::vector<glm::vec3>>(),triMesh.f()));
    object->centerlized();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    con<InteractiveCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = con<ShaderCtrl>().shader("core",true);
        auto view = con<ViewCtrl>().view();
        auto object = con<InteractiveCtrl>().object("coord");
        if(object->visible == false) return;
        int axes = object->m_v.f().size()/3;
        shader->bind();
        shader->setUniformValue("camera_vp", view->MatrixVP());
        shader->setUniformValue("model", view->Model()*object->Model());
        shader->setUniformValue("base_color", 1.0f, 0.0f, 0.0f);
        object->drawElementScript(0,axes);
        shader->setUniformValue("base_color", 0.0f, 1.0f, 0.0f);
        object->drawElementScript(axes,axes);
        shader->setUniformValue("base_color", 0.0f, 0.0f, 1.0f);
        object->drawElementScript(2*axes,axes);
        shader->release();
    });
//    auto cylinder_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/cylinder.obj";
//    YbMesh::IO::writePartialMesh(object->m_v, *f, cylinder_file);
}
