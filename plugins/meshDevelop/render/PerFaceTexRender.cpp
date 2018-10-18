#include "PerFaceTexRender.h"
#include <YbCore/scripts>

PerFaceTexRender::PerFaceTexRender(TriMesh& mesh,YbMesh::indicesTriMesh<glm::vec2>& texMesh)
    :InteractiveFaceTexObject(mesh,texMesh),
    RenderScript(YbCore::defaultScript::nullRender)
{
    this->normalize();
    this->calculateNorm();
    RenderScript([this](QTime&) {
        this->texture = new  QOpenGLTexture(QImage("/Users/hotpot/data/007/007Image1.png").mirrored());
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
    if(this->visible == false) return;
    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*this->Model());
    shader->setUniformValue("base_color", 0.3f, 0.3f, 0.3f);
    this->drawElementScript();
    shader->release();
}
