#include "Pick.h"
#include "../controller/ViewController.h"
#include "../controller/ShaderController.h"
#include "../controller/InteractiveController.h"
#include <easylogging++.h>
#include <iostream>
#include <QSGTextureProvider>
#include <QOpenGLFramebufferObject>
#include "../mesh/PickableMesh.h"

PickTool::PickTool():RenderScript(std::bind(&PickTool::drawResult, this, std::placeholders::_1))
{

}

void PickTool::clearPicks() {
    visible_picks.clear();
}

void PickTool::pickScript() {
    int pre_fbo = 0;
    gl.glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &pre_fbo);
    gl.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
    gl.glDisable(GL_DITHER);

    gl.glClearColor(-1.0f, 0.0f, 0.0f, 1.0f);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = con<ShaderCtrl>().shader("picker",true);
    auto view = con<ViewCtrl>().view();
    shader->bind();
    shader->setUniformValue("camera_vp", con<ViewCtrl>().view()->MatrixVP());

    for(auto objectKV:con<InteractiveCtrl>().allObjects()){
        auto mesh = objectKV.second;
        if(mesh->visible == false) continue;
        shader->setUniformValue("model", con<ViewCtrl>().view()->Model()*mesh->Model());
        shader->setUniformValue("mesh_id",(float)objectKV.first);
        mesh->drawElementScript();
    }
    shader->release();

    gl.glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
    gl.glReadBuffer(GL_COLOR_ATTACHMENT0);

    float rgb[2];
    for(auto& e: screen_coords){
//        LOG(INFO) <<" screen_coords : " << e[0] << ' ' << e[1];
        gl.glReadPixels(e[0], e[1], 1, 1, GL_RED, GL_FLOAT, rgb);
        gl.glReadPixels(e[0], e[1], 1, 1, GL_GREEN, GL_FLOAT, rgb+1);
//        LOG(INFO) <<" rgb : " << rgb[0] << ' ' << rgb[1];
        if(rgb[0] != -1)
            if(rgb[0] > -1.0f)
                visible_picks.push_back({(int)rgb[0],(int)rgb[1]});
    }

    gl.glEnable(GL_DITHER);
    gl.glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    gl.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, pre_fbo);
}

void PickTool::drawResult(QTime& t) {
    for(auto& e:visible_picks) {
        gl.glEnable(GL_POLYGON_OFFSET_FILL);
        gl.glPolygonOffset(-1.0,-1.0);
        auto shader = con<ShaderCtrl>().shader("selection",true);
        auto mesh   = con<InteractiveCtrl>().object(e[0]);
        shader->bind();
        shader->setUniformValue("camera_vp", con<ViewCtrl>().view()->MatrixVP());
        shader->setUniformValue("model", con<ViewCtrl>().view()->Model()*mesh->Model());
        mesh->drawElementScript(e[1], 1);
        shader->release();
        gl.glDisable(GL_POLYGON_OFFSET_FILL);
    }
}

void PickTool::createBufferScript() {
    gl.glGenFramebuffers(1, &this->m_fbo);
    gl.glGenRenderbuffers(2, this->m_rbo);
}
void PickTool::sizingScreenBufferScript() {
    auto size = con<RenderCtrl>().frameBufferObject().size();
    int pre_fbo = 0;
    gl.glGetIntegerv(GL_FRAMEBUFFER_BINDING, &pre_fbo);

    gl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    gl.glBindRenderbuffer(GL_RENDERBUFFER, m_rbo[0]);
    gl.glRenderbufferStorage(GL_RENDERBUFFER,GL_RG32F, size.width(), size.height());

    gl.glBindRenderbuffer(GL_RENDERBUFFER, m_rbo[1]);
    gl.glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24, size.width(), size.height());

    gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rbo[0]);
    gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo[1]);

    //    gl.glNamedRenderbufferStorage(m_rbo[0], GL_RG32F, x, y);
    //    gl.glNamedFramebufferRenderbuffer(m_fbo, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rbo[0]);

    //    gl.glNamedRenderbufferStorage(m_rbo[1], GL_DEPTH_COMPONENT24, x, y);
    //    gl.glNamedFramebufferRenderbuffer(m_fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rbo[1]);

    GLenum Status = gl.glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        printf("FB error, status: 0x%x\n", Status);
    }
    gl.glBindFramebuffer(GL_FRAMEBUFFER, pre_fbo);
}

int  PickTool::getSingleFacePick(int x, int y){
    screen_coords = {{x,y}};
    volatile int retu = -2;
    pick_lock.lockForWrite();
    RenderScript([&retu,this](QTime& t){
        pickScript();
        retu = visible_picks.size()?visible_picks[0][1]:-1;
        pick_lock.unlock();
    });
    pick_lock.lockForRead();
    pick_lock.unlock();
    clearPicks();
    return  retu;
}

void PickTool::meshPick(int x, int y) {
    screen_coords = {{x,y}};
    RenderScript([&](QTime& t){
        pickScript();
        if(visible_picks.size())
            con<InteractiveCtrl>().focus(con<InteractiveCtrl>().object(visible_picks[0][0]));
        else
            con<InteractiveCtrl>().focus(nullptr);
        clearPicks();
    });
}

