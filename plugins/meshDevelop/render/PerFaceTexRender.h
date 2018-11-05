#ifndef PERFACETEXRENDER_H
#define PERFACETEXRENDER_H

#include <QObject>
#include <YbCore/controller>
#include <glm/vec2.hpp>
#include <QOpenGLTexture>
class PluginBackend;


class PerFaceTexRender:public InteractiveFaceTexObject,public RenderScript
{
public:
    PerFaceTexRender(TriMesh& mesh, YbMesh::indicesTriMesh<glm::vec2>& texMesh,std::string tex);

    void draw(QTime& t);
};

#endif // PERFACETEXRENDER_H
