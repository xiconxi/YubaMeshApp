#include "FboNode.h"
#include "controller/RenderController.h"
#include "controller/ViewController.h"
#include "controller/ShaderController.h"
#include "controller/CentralController.h"
#include "controller/InteractiveController.h"

#include "utils/Pick.h"
#include "utils/Select.h"
#include "utils/SelectCanvas.h"
#include "utils/auxiliary.h"
//#include "utils/ScriptSamples.h"

#include "bases/InteractiveObjectMesh.h"

#include <QSGTextureProvider>
#include <easylogging++.h>
#include <QThread>

FboNode::FboNode()
{

}

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
       {"core"  ,GLSLFileConfig{V(prefix+"default"),F(prefix+"default")},nullptr},
       {"axes",GLSLFileConfig{V(prefix+"tex_axes"),F(prefix+"tex_axes")},nullptr},
       {"selection",GLSLFileConfig{V(prefix+"default"),F(prefix+"selection")},nullptr},
       {"select",GLSLFileConfig{V(prefix+"select_mask"),G(prefix+"select_mask")},[](GLuint id){
            const char* select_varing[] = {"face"};
            gl.glTransformFeedbackVaryings(id, 1, select_varing ,GL_INTERLEAVED_ATTRIBS);
        }}
    });

    global::con<ViewCtrl>().addView("cam",new ViewerMatrix());

    // meshCtrl Callback! must initialize
    global::con<InteractiveCtrl>().selectTool = new("selectTool") SelectTool();
    global::con<InteractiveCtrl>().selectTool->createBufferScript();

    global::con<InteractiveCtrl>().pickTool = new("pickTool") PickTool();
    global::con<InteractiveCtrl>().pickTool->createBufferScript();

    YbCore::aux::addCoord3d(1.0f,0.01f,"axes");
    YbCore::aux::addBox3d(10.0f,0.1f,"box");

    return ret;
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
    global::con<InteractiveCtrl>().pickTool->meshPick(x,y);
    global::con<RenderCtrl>().update();
}

void FboNode::screenAreaPick(QQuickPaintedItem *item) {
    global::con<InteractiveCtrl>().selectTool->areasFaceSelect(item);
    global::con<RenderCtrl>().update();
    ((SelectCanvas*)item)->clear();
}

void FboNode::axesVisible(bool status) {
    global::con<InteractiveCtrl>().object("axes")->visible = status;
    global::con<RenderCtrl>().update();
}

void FboNode::boxVisible(bool status) {
    global::con<InteractiveCtrl>().object("box")->visible = status;
    global::con<RenderCtrl>().update();
}
