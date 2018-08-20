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

class LIBSHARED_EXPORT PickableMesh:public QObject
{
    Q_OBJECT
public:
    // vertex position, face
    // vertex array object, indices, vertex property
    std::vector<glm::vec3>   v;
    std::vector<glm::vec3>   vn;
    std::vector<glm::ivec3>  f;
    uint vao, ibo, v_buffer, vn_buffer;

    // model
    glm::mat4 model;
    QMatrix4x4 Model();
    void rotate(int dx, int dy);
    void scaleBy(float s);

    PickableMesh();
    virtual ~PickableMesh();

signals:
   void Selected(PickableMesh* mesh);
public:
    virtual void createBufferScript();
    virtual void syncVertexBuffersDataScript();
    void syncFacesBuffersDataScript();
    void drawElementScript(uint start=0, uint size=0);
    void drawElementBufferScript(uint buffer_id, uint start=0, uint size=0);

    bool visible = true;
    uint numericalId() const{return numerical_id;}
private:
    uint numerical_id;
    static uint numerical_mesh_amount;
};

#endif // PICKABLEMESH_H
