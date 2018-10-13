#ifndef IINTERACTIVEOBJECT_H
#define IINTERACTIVEOBJECT_H
#include "../bases/glviewerGlobal.inc"
#include <QObject>
#include <YbMesh/YbMesh.hpp>
#include "../bases/GLMeshObject.h"

class SelectTool;

class LIBSHARED_EXPORT InteractiveObject: public QObject,public IGLMeshObject{
    Q_OBJECT
public:
    InteractiveObject(TriMesh&& vmesh, int components = 1);
    void createBufferScript() override;
    void syncSelectBufferScript();

    virtual void selectScript(QTime &t);
    void selectionVisualizedScript(QTime& t);
    ~InteractiveObject();

    const std::vector<glm::ivec3>& selectedFaces();
signals:
    void FaceSelected(InteractiveObject* object);
protected:
    void downloadSelectedBufferScript(int buffer_size);
    friend class SelectTool;
    uint selected_buffer = 0;
    std::vector<glm::ivec3> selected_faces;
    std::string select_shader_name = "select";
};


#endif // IINTERACTIVEOBJECT_H
