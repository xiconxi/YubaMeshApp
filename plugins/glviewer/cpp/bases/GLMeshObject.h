#ifndef PICKABLEMESH_H
#define PICKABLEMESH_H

#include "../bases/glviewerGlobal.inc"
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <QMatrix4x4>
#include <stdint.h>
#include <QObject>
#include <YbMesh/YbMesh.hpp>
class QOpenGLTexture;

class LIBSHARED_EXPORT IModelTransform{
public:
    glm::mat4 model;
    QMatrix4x4 Model();
    void rotate(int dx, int dy);
    void scaleBy(float s);
};

class LIBSHARED_EXPORT IGLMeshObject: public IModelTransform{
public:
    typedef YbMesh::indicesTriMesh<glm::vec3> TriMesh;
    IGLMeshObject(TriMesh&& vmesh, int components = 1);
    // GL functions
    virtual void createBufferScript();
    virtual void syncVertexBuffersDataScript();
    virtual void syncFacesBuffersDataScript();
    void drawElementScript(uint start=0, uint size=0);
    virtual void drawElementBufferScript(uint buffer_id, uint start=0, uint size=0);
    virtual void multiDrawElementScript();
    virtual ~IGLMeshObject();

    void calculateNorm();
    void normalize(bool centralized = true);

    YbMesh::indicesTriMesh<glm::vec3> m_v;
    YbMesh::indicesTriMesh<glm::vec3> m_n;

    struct LIBSHARED_EXPORT _{
        _(int s,int ss);
        std::vector<glm::ivec2> intervals;
        void update();
    private:
        friend class IGLMeshObject;
        std::vector<int> counts;
        std::vector<const void*> offset;
    }components;

    QOpenGLTexture* texture;
protected:
    uint vao, ibo, v_buffer;
};

#endif // PICKABLEMESH_H


