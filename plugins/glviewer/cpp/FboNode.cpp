#include "FboNode.h"
#include "controller/RenderController.h"
#include "controller/ViewController.h"
#include "controller/ShaderController.h"
#include "controller/CentralController.h"
#include "controller/GLMeshController.h"

#include "utils/Pick.h"
#include "utils/Select.h"
#include "utils/SelectCanvas.h"
#include "utils/auxiliary.h"
//#include "utils/ScriptSamples.h"

#include "bases/GLMeshObject.h"
#include "render/InteractiveObject.h"

#include <QSGTextureProvider>
#include <easylogging++.h>
#include <QThread>
#include <QFileInfo>
#include <QOpenGLTexture>


FboNode::FboNode()
{

}

std::string launchUrl;

void FboNode::set_fps(qreal time) const{
    static qreal last_time = 0;
    static int frame = 59;
    frame++;
    if(time > last_time){
        m_fps = frame;
        frame = 0;
        last_time = time;
        emit const_cast<FboNode* const>(this)->fpsChanged();
    }
}

QQuickFramebufferObject::Renderer* FboNode::createRenderer() const {
    global::con<CentralCtrl>().setFboNode(this);
    auto ret =  &global::con<RenderCtrl>();

    QString prefix = PLUGINPATH+QString("GLViewer/glsl/");
    global::con<ShaderCtrl>().addShaderProgram({
       {"picker",GLSLFileConfig{V(prefix+"picker"),F(prefix+"picker")},nullptr},
       {"core"  ,GLSLFileConfig{V(prefix+"default"),G(prefix+"default"),F(prefix+"default")},nullptr},
       {"axes",GLSLFileConfig{V(prefix+"tex_axes"),F(prefix+"tex_axes")},nullptr},
       {"selection",GLSLFileConfig{V(prefix+"default"),F(prefix+"selection")},nullptr},
       {"adjacency", GLSLFileConfig{V(prefix+"adjacency_tex"),G(prefix+"adjacency_tex"),F(prefix+"adjacency_tex") },nullptr},
       {"select",GLSLFileConfig{V(prefix+"select_mask"),G(prefix+"select_mask")},[](GLuint id){
            const char* select_varing[] = {"face"};
            gl.glTransformFeedbackVaryings(id, 1, select_varing ,GL_INTERLEAVED_ATTRIBS);
        }},
        {"select_adjacency",GLSLFileConfig{V(prefix+"select_mask"),G(prefix+"select_mask_adjacency")},[](GLuint id){
            const char* select_varing[] = {"face"};
            gl.glTransformFeedbackVaryings(id, 1, select_varing ,GL_INTERLEAVED_ATTRIBS);
        }}

    });

    global::con<ViewCtrl>().addView("cam",new ViewerMatrix());

    // meshCtrl Callback! must initialize
    global::con<GLMeshCtrl>().selectTool = new("selectTool") SelectTool();
    global::con<GLMeshCtrl>().selectTool->createBufferScript();

    global::con<GLMeshCtrl>().pickTool = new("pickTool") PickTool();
    global::con<GLMeshCtrl>().pickTool->createBufferScript();

    YbCore::aux::addCoord3d(1.0f,0.01f,"axes");
    YbCore::aux::addBox3d(10.0f,0.1f,"box");
//    YbCore::aux::addGridBoard(10.0f, 10.0f, 2.0f, "board");

//    YbCore::aux::addInteractiveFaceTexDemo("/Users/hotpot/data/007/007.obj", "demo");
//    YbCore::aux::addInteractiveDemo("/Users/hotpot/data/007/007.obj", "demo");

    is_ready = true;

    if(launchUrl.size() == 0 ) return ret;

    auto object = new InteractiveObject(YbMesh::IO::importOBJ_V0(launchUrl));
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        QString tex = launchUrl.substr(0,launchUrl.find(".obj")).c_str()+QString("Image1.png");
        if( QFileInfo( tex ).exists() ){
            object->texture = new  QOpenGLTexture(QImage(tex.toStdString().c_str()).mirrored());
            object->texture->bind();
            object->texture->setMinificationFilter(QOpenGLTexture::Nearest);
            object->texture->setMagnificationFilter(QOpenGLTexture::Linear);
            object->texture->setWrapMode(QOpenGLTexture::Repeat);
            object->texture->release();
        }
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
        object->syncSelectBufferScript();
    });
    global::con<GLMeshCtrl>().addInteractiveObject(launchUrl.substr(launchUrl.find_last_of("/")), object);

    return ret;
}


void FboNode::importMesh(QString qurl) {
    launchUrl = qurl.toStdString();
    if(is_ready == false) return ;
    auto object = new InteractiveObject(YbMesh::IO::importOBJ_V0(launchUrl));
    object->normalize();
    object->calculateNorm();
    RenderScript([=](QTime&) {
        QString tex = launchUrl.substr(0,launchUrl.find(".obj")).c_str()+QString("Image1.png");
        if( QFileInfo( tex ).exists() ){
            object->texture = new  QOpenGLTexture(QImage(tex.toStdString().c_str()).mirrored());
            object->texture->bind();
            object->texture->setMinificationFilter(QOpenGLTexture::Nearest);
            object->texture->setMagnificationFilter(QOpenGLTexture::Linear);
            object->texture->setWrapMode(QOpenGLTexture::Repeat);
            object->texture->release();
        }
        object->createBufferScript();
        object->syncVertexBuffersDataScript();
        object->syncFacesBuffersDataScript();
        object->syncSelectBufferScript();
    });
    global::con<GLMeshCtrl>().addInteractiveObject(launchUrl.substr(launchUrl.find_last_of("/")), object);
}

void FboNode::move(int x, int y) {
    global::con<RenderCtrl>().render_lock.lockForRead();
    global::con<ViewCtrl>().view()->move(x, y);
    global::con<RenderCtrl>().update();
    global::con<RenderCtrl>().render_lock.unlock();
}

void FboNode::rotate(int x, int y) {
    global::con<RenderCtrl>().render_lock.lockForRead();
    global::con<ViewCtrl>().view()->rotate(x, y);
    global::con<RenderCtrl>().update();
    global::con<RenderCtrl>().render_lock.unlock();
}

void FboNode::scaleBy(float s) {
    global::con<RenderCtrl>().render_lock.lockForRead();
    global::con<ViewCtrl>().view()->scaleBy(s);
    global::con<RenderCtrl>().update();
    global::con<RenderCtrl>().render_lock.unlock();
}

void FboNode::singleFacePick(int x, int y) {
    global::con<GLMeshCtrl>().pickTool->meshPick(x,y);
    global::con<RenderCtrl>().update();
}

void FboNode::screenAreaPick(QQuickPaintedItem *item) {
    global::con<GLMeshCtrl>().selectTool->areasFaceSelect(item);
    global::con<RenderCtrl>().update();
    ((SelectCanvas*)item)->clear();
}

void FboNode::axesVisible(bool status) {
    status? global::con<RenderCtrl>().resume("axes"): global::con<RenderCtrl>().pause("axes");
    global::con<RenderCtrl>().update();
}

void FboNode::boxVisible(bool status) {
    status? global::con<RenderCtrl>().resume("box"): global::con<RenderCtrl>().pause("box");
    global::con<RenderCtrl>().update();
}

