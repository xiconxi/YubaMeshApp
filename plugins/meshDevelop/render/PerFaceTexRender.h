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
    PerFaceTexRender(TriMesh& mesh, YbMesh::indicesTriMesh<glm::vec2>& texMesh);

    // GL functions
//    void syncVertexBuffersDataScript() override;
//    void syncFacesBuffersDataScript() override;

//    void drawElementBufferScript(uint buffer_id, uint start=0, uint size=0) override;

    void draw(QTime& t);
private:
//    YbMesh::indicesTriMesh<glm::vec2> texM;
};

#endif // PERFACETEXRENDER_H
