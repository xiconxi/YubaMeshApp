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

class InteractiveObject;

class LIBSHARED_EXPORT SelectTool: public RenderScript {
public:
    SelectTool();

    void areasFaceSelect(QQuickPaintedItem* areaItem);

    void createBufferScript();

    void clearSelect();

    void syncBufferScript(InteractiveObject* object);

    std::vector<glm::ivec3>& getSelectedFace();

private:

    void SelectScript(QTime& t);

    void drawResultSrcipt(QTime& t);

    std::vector<glm::ivec3> downloadSelectionsScript(InteractiveObject* object);

    void beginStreamQueryScript();

    void endStreamQueryScript();

    QQuickPaintedItem* areaItem;

    GLuint xfb = 0, face_vao;

    std::array<uint,2>  queries = {0,0};
    std::array<int,2>  stream_size = {0,0};

    QReadWriteLock select_lock;
};

#endif // SELECTMANAGER_H
