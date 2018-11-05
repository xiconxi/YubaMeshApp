#include "auxiliary.h"
#include <YbMesh/YbMesh.hpp>
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/GLMeshController.h"
#include "../controller/RenderController.h"
#include "../bases/GLMeshObject.h"
#include "../render/InteractiveObject.h"
#include "../render/InteractiveFaceTexObject.h"
#include "ScriptSamples.h"

void YbCore::aux::addCoord3d(float h, float r, std::string name) {
    class _:public RenderScript{
    public:
        _(float h, float r):RenderScript(YbCore::defaultScript::nullRender),object(IGLMeshObject(YbMesh::geometry::make_axes(h,r))){
            object.normalize(false);
            object.calculateNorm();
            RenderScript([=](QTime&) {
                object.createBufferScript();
                object.syncVertexBuffersDataScript();
                object.syncFacesBuffersDataScript();
            });
            this->setRender(std::bind(&_::f, this, std::placeholders::_1));
        }
        void f(QTime& t){
            auto shader = global::con<ShaderCtrl>().shader("axes");
            auto view = global::con<ViewCtrl>().view();
            int axes = object.m_v.f().size()/3;
            shader->bind();
            shader->setUniformValue("camera_vp", view->MatrixVP());
            shader->setUniformValue("model", view->Model()*object.Model());
            shader->setUniformValue("base_color", 0.3f, 0.0f, 0.0f);
            object.drawElementScript(0,axes);
            shader->setUniformValue("base_color", 0.0f, 0.3f, 0.0f);
            object.drawElementScript(axes,axes);
            shader->setUniformValue("base_color", 0.0f, 0.0f, 0.3f);
            object.drawElementScript(2*axes,axes);
            shader->release();
        }
    private:
        IGLMeshObject object;
    };
    new(name) _(h,r);

//    auto cylinder_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/cylinder.obj";
//    YbMesh::IO::writePartialMesh(object->m_v, *f, cylinder_file);
}
void YbCore::aux::addBox3d(float h, float r, std::string name) {

    class _:public RenderScript{
    public:
        _(float h, float r):RenderScript(YbCore::defaultScript::nullRender),object(IGLMeshObject(YbMesh::geometry::make_box(h,r))){
            object.normalize(false);
            object.calculateNorm();
            RenderScript([=](QTime&) {
                object.createBufferScript();
                object.syncVertexBuffersDataScript();
                object.syncFacesBuffersDataScript();
            });
            this->setRender(std::bind(&_::f, this, std::placeholders::_1));
        }
        void f(QTime& t){
            auto shader = global::con<ShaderCtrl>().shader("core");
            auto view = global::con<ViewCtrl>().view();
            shader->bind();
            shader->setUniformValue("camera_vp", view->MatrixVP());
            shader->setUniformValue("model", view->Model()*object.Model());
            shader->setUniformValue("base_color", 0.3f, 0.0f, 0.0f);
            object.drawElementScript(0,640);
            shader->setUniformValue("base_color", 0.0f, 0.3f, 0.0f);
            object.drawElementScript(640,640);
            shader->setUniformValue("base_color", 0.0f, 0.0f, 0.3f);
            object.drawElementScript(1280,640);
            shader->setUniformValue("base_color", 0.3f, 0.3f, 0.0f);
            object.drawElementScript(1920,2400-1920);
            shader->release();
        }
    private:
        IGLMeshObject object;
    };
    new(name) _(h,r);
    //    auto cylinder_file = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/cylinder.obj";
    //    YbMesh::IO::writePartialMesh(object->m_v, *f, cylinder_file);
}
#include <QStandardPaths>
void YbCore::aux::addGridBoard(float w, float h, float gap, std::string name) {
    class _:public RenderScript{
    public:
        _(float w, float h, float gap):RenderScript(YbCore::defaultScript::nullRender),object(IGLMeshObject(YbMesh::geometry::make_plane(w,h,gap))){
            object.normalize(false);
            object.calculateNorm();
            RenderScript([=](QTime&) {
                object.createBufferScript();
                object.syncVertexBuffersDataScript();
                object.syncFacesBuffersDataScript();
            });
            this->setRender(std::bind(&_::f, this, std::placeholders::_1));
        }
        void f(QTime& t){
            auto shader = global::con<ShaderCtrl>().shader("core");
            auto view = global::con<ViewCtrl>().view();
            shader->bind();
            shader->setUniformValue("camera_vp", view->MatrixVP());
            shader->setUniformValue("model", view->Model()*object.Model());
            shader->setUniformValue("base_color", 0.3f, 0.3f, 0.3f);
            object.drawElementScript();
            shader->release();
        }
        IGLMeshObject object;
    };
    auto _1 = new(name) _(w,h,gap);
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
        shader->bind();
        shader->setUniformValue("camera_vp", view->MatrixVP());
        shader->setUniformValue("model", view->Model()*object->Model());
        shader->setUniformValue("base_color", 0.3f, 0.3f, 0.3f);
        object->drawElementScript();
        shader->release();
    });
}
