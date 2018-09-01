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

class IModelTransform{
public:
    glm::mat4 model;
    QMatrix4x4 Model();
    void rotate(int dx, int dy);
    void scaleBy(float s);
};

class IDrawObject: public IModelTransform{
public:
    typedef YbMesh::indicesTriMesh<glm::vec3> TriMesh;
    IDrawObject(TriMesh vmesh,TriMesh nmesh):m_v(vmesh),m_n(nmesh){}
    // GL functions
    virtual void createBufferScript();
    virtual void syncVertexBuffersDataScript();
    void syncFacesBuffersDataScript();
    void drawElementScript(uint start=0, uint size=0);
    void drawElementBufferScript(uint buffer_id, uint start=0, uint size=0);
    ~IDrawObject(){}

    void calculateNorm();
    void centerlized();

    bool visible = true;

    YbMesh::indicesTriMesh<glm::vec3> m_v;

protected:
    uint vao, ibo, v_buffer;

    YbMesh::indicesTriMesh<glm::vec3> m_n;
};

class InteractiveObject: public QObject,public IDrawObject{
    Q_OBJECT
public:
    InteractiveObject(TriMesh vmesh,TriMesh nmesh);
    ~InteractiveObject();
signals:
    void FaceSelected(IDrawObject* object);
};

#endif // PICKABLEMESH_H


