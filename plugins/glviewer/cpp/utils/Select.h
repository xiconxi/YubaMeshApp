#ifndef SELECTMANAGER_H
#define SELECTMANAGER_H
#include <QOpenGLBuffer>
#include <functional>
#include <vector>
#include <QQuickPaintedItem>
#include "../controller/RenderController.h"
#include <stdint.h>
#include <array>
#include <glm/vec3.hpp>
#include <QReadWriteLock>

class LIBSHARED_EXPORT SelectTool: public RenderScript {
public:
    SelectTool();

    void areasFaceSelect(QQuickPaintedItem* areaItem);

    void createBufferScript();

    void sizingBufferScriptWrap(GLuint f_size);

    void clearSelect();

    std::vector<glm::ivec3>& getSelectedFace();

private:

    void SelectScript(QTime& t);

    void drawResultSrcipt(QTime& t);

    void syncBufferScript(QTime& t);

    void downloadSelectionsScript();

    void beginStreamQueryScript();

    void endStreamQueryScript();

    QQuickPaintedItem* areaItem;
    std::vector<std::array<uint,2> > selections;

    GLuint xfb = 0,face_buffer, face_vao;

    std::array<uint,2>  queries = {0,0};
    std::array<int,2>  stream_size;

    uint current_face_buffer_size = 0;
    std::vector<glm::ivec3> selected_faces;

    QReadWriteLock select_lock;
};

#endif // SELECTMANAGER_H
