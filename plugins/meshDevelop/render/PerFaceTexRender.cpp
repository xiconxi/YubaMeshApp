#include "PerFaceTexRender.h"
#include <YbCore/scripts>

PerFaceTexRender::PerFaceTexRender(TriMesh& mesh,YbMesh::indicesTriMesh<glm::vec2>& texMesh,std::string tex)
    :InteractiveFaceTexObject(mesh,texMesh),
    RenderScript(YbCore::defaultScript::nullRender)
{
    this->normalize();
    this->calculateNorm();
    RenderScript([this,tex](QTime&) {
        this->texture = new  QOpenGLTexture(QImage(tex.c_str()).mirrored());
        this->texture->bind();
        this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
        this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
        this->texture->setWrapMode(QOpenGLTexture::Repeat);
        this->texture->release();

        this->createBufferScript();
        this->syncVertexBuffersDataScript();
        this->syncFacesBuffersDataScript();
        this->syncSelectBufferScript();
    });
    setRender(std::bind(&PerFaceTexRender::draw, this, std::placeholders::_1));
}

void PerFaceTexRender::draw(QTime &t) {
    auto shader = global::con<ShaderCtrl>().shader("adjacency");
    auto view = global::con<ViewCtrl>().view();
//    if(this->visible == false) return;
    this->texture->bind();
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*this->Model());
    shader->setUniformValue("base_color", 0.3f, 0.3f, 0.3f);
    this->drawElementScript();
    this->texture->release();
    shader->release();
}
