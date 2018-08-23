#include "FboNode.h"
#include "controller/RenderController.h"
#include "controller/ViewController.h"
#include "controller/MeshController.h"
#include "controller/ShaderController.h"
#include "controller/CentralController.h"

#include "mesh/meshProcessing.h"
#include "mesh/meshCodes.h"

#include "utils/Pick.h"
#include "utils/Select.h"
#include "utils/SelectCanvas.h"
#include "utils/ScriptSamples.h"

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
    con<CentralCtrl>().selectTool = new("selectTool") SelectTool();
    con<CentralCtrl>().selectTool->createBufferScript();

    con<CentralCtrl>().pickTool = new("pickTool") PickTool();
    con<CentralCtrl>().pickTool->createBufferScript();

#ifdef LOAD_BUNNY
    auto mesh = YbCore::IO::readObj(MESHPATH"bunny.obj");
    YbCore::calculateNorm(mesh);
    YbCore::centerlized(mesh);

    RenderScript([=](QTime&) {
        mesh->createBufferScript();
        mesh->syncVertexBuffersDataScript();
        mesh->syncFacesBuffersDataScript();
    });

    con<MeshCtrl>().addMesh("bunny",mesh); //bunny FullBodyScan 20180205142827.cie
#endif
    return ret;
}

void FboNode::move(int x, int y) {
    con<ViewCtrl>().view()->move(x, y);
    con<RenderCtrl>().update();
}

void FboNode::rotate(int x, int y) {
    con<ViewCtrl>().view()->rotate(x, y);
    con<RenderCtrl>().update();
}

void FboNode::scaleBy(float s) {
    con<ViewCtrl>().view()->scaleBy(s);
    con<RenderCtrl>().update();
}

void FboNode::registerModule(QString module) {
    con<CentralCtrl>().setModuleName(module.toStdString());
}

void FboNode::unregisterModule(QString module) {
    con<CentralCtrl>().releaseModuleResource(module.toStdString());
}

void FboNode::singleFacePick(int x, int y) {
    con<CentralCtrl>().pickTool->meshPick(x,y);
    con<RenderCtrl>().update();
}

void FboNode::screenAreaPick(QQuickPaintedItem *item) {
    con<CentralCtrl>().selectTool->areasFaceSelect(item);
    con<RenderCtrl>().update();
    ((SelectCanvas*)item)->clear();
}
