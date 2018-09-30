#include "auxiliary.h"
#include <YbMesh/YbMesh.hpp>
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/InteractiveController.h"
#include "../controller/RenderController.h"
#include "../bases/InteractiveObjectMesh.h"
void YbCore::aux::addCoord3d(float h, float r, std::string name) {
    auto object = new IDrawObject(YbMesh::geometry::make_axes(h,r));
    object->normalize(false);
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    global::con<InteractiveCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = global::con<ShaderCtrl>().shader("axes");
        auto view = global::con<ViewCtrl>().view();
        auto object = global::con<InteractiveCtrl>().object(name);
        if(object->visible == false) return;
        int axes = object->m_v.f().size()/3;
        shader->bind();
        shader->setUniformValue("camera_vp", view->MatrixVP());
        shader->setUniformValue("model", view->Model()*object->Model());
        shader->setUniformValue("base_color", 0.3f, 0.0f, 0.0f);
        object->drawElementScript(0,axes);
        shader->setUniformValue("base_color", 0.0f, 0.3f, 0.0f);
        object->drawElementScript(axes,axes);
        shader->setUniformValue("base_color", 0.0f, 0.0f, 0.3f);
        object->drawElementScript(2*axes,axes);
        shader->release();
    });
//    auto cylinder_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/cylinder.obj";
//    YbMesh::IO::writePartialMesh(object->m_v, *f, cylinder_file);
}
void YbCore::aux::addBox3d(float h, float r, std::string name) {
    auto object = new IDrawObject(YbMesh::geometry::make_box(h,r));
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    global::con<InteractiveCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = global::con<ShaderCtrl>().shader("core");
        auto view = global::con<ViewCtrl>().view();
        auto object = global::con<InteractiveCtrl>().object(name);
        if(object->visible == false) return;
        shader->bind();
        shader->setUniformValue("camera_vp", view->MatrixVP());
        shader->setUniformValue("model", view->Model()*object->Model());
        shader->setUniformValue("base_color", 0.3f, 0.0f, 0.0f);
        object->drawElementScript(0,640);
        shader->setUniformValue("base_color", 0.0f, 0.3f, 0.0f);
        object->drawElementScript(640,640);
        shader->setUniformValue("base_color", 0.0f, 0.0f, 0.3f);
        object->drawElementScript(1280,640);
        shader->setUniformValue("base_color", 0.3f, 0.3f, 0.0f);
        object->drawElementScript(1920,2400-1920);
        shader->release();
    });
    //    auto cylinder_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/cylinder.obj";
    //    YbMesh::IO::writePartialMesh(object->m_v, *f, cylinder_file);
}
