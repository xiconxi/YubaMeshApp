#include "auxiliary.h"
#include <YbMesh/YbMesh.hpp>
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/GLMeshController.h"
#include "../controller/RenderController.h"
#include "../bases/GLMeshObject.h"
#include "../render/InteractiveObject.h"
#include "../render/InteractiveFaceTexObject.h"

void YbCore::aux::addCoord3d(float h, float r, std::string name) {
    auto object = new IGLMeshObject(YbMesh::geometry::make_axes(h,r));
    object->normalize(false);
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    global::con<GLMeshCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = global::con<ShaderCtrl>().shader("axes");
        auto view = global::con<ViewCtrl>().view();
        auto object = global::con<GLMeshCtrl>().object(name);
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
    auto object = new IGLMeshObject(YbMesh::geometry::make_box(h,r));
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
    });
    global::con<GLMeshCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = global::con<ShaderCtrl>().shader("core");
        auto view = global::con<ViewCtrl>().view();
        auto object = global::con<GLMeshCtrl>().object(name);
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

#include <QImage>
#include <QOpenGLTexture>

void YbCore::aux::addInteractiveDemo(std::string filename, std::string name) {
    auto object = new InteractiveObject(YbMesh::IO::importOBJ_V0(filename));
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
        object->syncSelectBufferScript();
    });
    global::con<GLMeshCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = global::con<ShaderCtrl>().shader("core");
        auto view = global::con<ViewCtrl>().view();
        auto object = global::con<GLMeshCtrl>().object(name);
        if(object->visible == false) return;
        shader->bind();
        shader->setUniformValue("camera_vp", view->MatrixVP());
        shader->setUniformValue("model", view->Model()*object->Model());
        shader->setUniformValue("base_color", 0.3f, 0.3f, 0.3f);
        object->drawElementScript();
        shader->release();
    });
}

void YbCore::aux::addInteractiveFaceTexDemo(std::string filename, std::string name) {
    auto dual_mesh = YbMesh::IO::importOBJ_V1(filename);
    auto object = new InteractiveFaceTexObject(dual_mesh.second, dual_mesh.first);
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
        object->syncSelectBufferScript();
    });
    global::con<GLMeshCtrl>().addInteractiveObject(name, object);

    new(name) RenderScript([=](QTime&) {
        auto shader = global::con<ShaderCtrl>().shader("adjacency");
        auto view = global::con<ViewCtrl>().view();
        auto object = global::con<GLMeshCtrl>().object(name);
        if(object->visible == false) return;
        shader->bind();
        shader->setUniformValue("camera_vp", view->MatrixVP());
        shader->setUniformValue("model", view->Model()*object->Model());
        shader->setUniformValue("base_color", 0.3f, 0.3f, 0.3f);
        object->drawElementScript();
        shader->release();
    });
}
