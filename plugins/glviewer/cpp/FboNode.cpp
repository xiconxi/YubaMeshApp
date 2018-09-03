#include "FboNode.h"
#include "controller/RenderController.h"
#include "controller/ViewController.h"
#include "controller/ShaderController.h"
#include "controller/CentralController.h"
#include "controller/InteractiveController.h"

#include "utils/Pick.h"
#include "utils/Select.h"
#include "utils/SelectCanvas.h"
//#include "utils/ScriptSamples.h"

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
    con<CentralCtrl>().setFboNode(this);
    auto ret =  &con<RenderCtrl>();

    con<ShaderCtrl>().initialize();
    con<ViewCtrl>().addView("cam",new ViewerMatrix());

    // meshCtrl Callback! must initialize
    con<InteractiveCtrl>().selectTool = new("selectTool") SelectTool();
    con<InteractiveCtrl>().selectTool->createBufferScript();

    con<InteractiveCtrl>().pickTool = new("pickTool") PickTool();
    con<InteractiveCtrl>().pickTool->createBufferScript();

    return ret;
}

void FboNode::move(int x, int y) {
    con<RenderCtrl>().render_lock.lockForRead();
    con<ViewCtrl>().view()->move(x, y);
    con<RenderCtrl>().update();
    con<RenderCtrl>().render_lock.unlock();
}

void FboNode::rotate(int x, int y) {
    con<RenderCtrl>().render_lock.lockForRead();
    con<ViewCtrl>().view()->rotate(x, y);
    con<RenderCtrl>().update();
    con<RenderCtrl>().render_lock.unlock();
}

void FboNode::scaleBy(float s) {
    con<RenderCtrl>().render_lock.lockForRead();
    con<ViewCtrl>().view()->scaleBy(s);
    con<RenderCtrl>().update();
    con<RenderCtrl>().render_lock.unlock();
}

void FboNode::registerModule(QString module) {
    con<CentralCtrl>().setModuleName(module.toStdString());
}

void FboNode::unregisterModule(QString module) {
    con<CentralCtrl>().releaseModuleResource(module.toStdString());
}

void FboNode::singleFacePick(int x, int y) {
    con<InteractiveCtrl>().pickTool->meshPick(x,y);
    con<RenderCtrl>().update();
}

void FboNode::screenAreaPick(QQuickPaintedItem *item) {
    con<InteractiveCtrl>().selectTool->areasFaceSelect(item);
    con<RenderCtrl>().update();
    ((SelectCanvas*)item)->clear();
}
