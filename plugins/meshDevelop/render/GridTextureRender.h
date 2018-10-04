#ifndef GRIDTEXTURERENDER_H
#define GRIDTEXTURERENDER_H

#include <QObject>
#include <YbCore/controller>
#include <glm/vec2.hpp>
#include <QOpenGLTexture>
class PluginBackend;

class GridTextureRender:public IDrawObject,public RenderScript
{
public:
    GridTextureRender(TriMesh& mesh);

    // GL functions
    void syncVertexBuffersDataScript() override;
private:
    void draw(QTime& t);

    friend class PluginBackend;
    YbMesh::share_vec<glm::vec2> vt_p;
    QOpenGLTexture* texture;
};

#endif // GRIDTEXTURERENDER_H
