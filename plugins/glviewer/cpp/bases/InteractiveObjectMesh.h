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

class LIBSHARED_EXPORT IModelTransform{
public:
    glm::mat4 model;
    QMatrix4x4 Model();
    void rotate(int dx, int dy);
    void scaleBy(float s);
};

class LIBSHARED_EXPORT IDrawObject: public IModelTransform{
public:
    typedef YbMesh::indicesTriMesh<glm::vec3> TriMesh;
    IDrawObject(TriMesh vmesh,TriMesh nmesh, int components = 1):m_v(vmesh),m_n(nmesh),
                components(components,components == 1? m_v.f().size():0){}
    // GL functions
    virtual void createBufferScript();
    virtual void syncVertexBuffersDataScript();
    void syncFacesBuffersDataScript();
    void drawElementScript(uint start=0, uint size=0);
    void drawElementBufferScript(uint buffer_id, uint start=0, uint size=0);
    void multiDrawElementScript();
    ~IDrawObject(){}

    void calculateNorm();
    void normalize(bool centralized = true);

    bool visible = true;

    YbMesh::indicesTriMesh<glm::vec3> m_v;
    YbMesh::indicesTriMesh<glm::vec3> m_n;

    struct LIBSHARED_EXPORT _{
        _(int s,int ss):intervals(s,glm::ivec2(0,ss)), counts(s,ss), offset(s,(const void*)0){}
        std::vector<glm::ivec2> intervals;
        void update();
    private:
        friend class IDrawObject;
        std::vector<int> counts;
        std::vector<const void*> offset;
    }components;

protected:
    uint vao, ibo, v_buffer;
};
class SelectTool;
class LIBSHARED_EXPORT InteractiveObject: public QObject,public IDrawObject{
    Q_OBJECT
public:
    InteractiveObject(TriMesh vmesh,TriMesh nmesh, int components = 1);
    void createBufferScript() override;
    void syncSelectBufferScript();
    ~InteractiveObject();
    const std::vector<glm::ivec3>& selectedFaces();
signals:
    void FaceSelected(InteractiveObject* object);
private:
    void downloadSelectedBufferScript(int buffer_size);
    friend class SelectTool;
    uint selected_buffer = 0;
    std::vector<glm::ivec3> selected_faces;
};

#endif // PICKABLEMESH_H


