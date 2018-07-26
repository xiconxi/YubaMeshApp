#ifndef MESHOBJECT_H
#define MESHOBJECT_H

#include "meshCodes.h"
#include <QOpenGLTexture>

class RenderMesh
{
public:
    RenderMesh(std::string filename);

    RenderMesh(glFormatMesh&& mesh);

    void indicesDraw(int start=0, int count=-1,GLint element_buffer=-1);

    QMatrix4x4 Model(){
        return mesh.Model();
    }

    QOpenGLTexture* texture = nullptr;

    void initialize();

    void syncGLVertexArrays();

    glFormatMesh* Mesh();

private:
    void initGLVertexArrays();
    void initGLTexture();
    void setTexture(std::string file);
    glFormatMesh mesh;
    QImage texture_qimg;

    std::pair<int,int> edge(int x,int y);
};

#endif // MESHOBJECT_H
