#ifndef DEVELOP_BACKEND_H
#define DEVELOP_BACKEND_H

#include <QObject>
#include <YbCore/controller>
#include <glm/vec2.hpp>

class PluginBackend;

class ScanRender:public RenderScript{
public:
    ScanRender();
private:
    float percent = 0;
    void draw(QTime& t);

    friend class PluginBackend;
};
#endif // DEVELOP_BACKEND_H
