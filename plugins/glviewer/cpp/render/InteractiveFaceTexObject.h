#ifndef INTERACTIVEFACETEXOBJECT_H
#define INTERACTIVEFACETEXOBJECT_H

#include "./InteractiveObject.h"

class InteractiveFaceTexObject: public InteractiveObject
{
public:
    InteractiveFaceTexObject(TriMesh& mesh, YbMesh::indicesTriMesh<glm::vec2>& texMesh);

    // draw call override
    void syncVertexBuffersDataScript() override;
    void syncFacesBuffersDataScript() override;

    void drawElementBufferScript(uint buffer_id, uint start=0, uint size=0) override;

    // selection override
    void selectScript(QTime &t) override;

private:
    YbMesh::indicesTriMesh<glm::vec2> texM;
};

#endif // INTERACTIVEFACETEXOBJECT_H
