#ifndef DEVELOP_BACKEND_H
#define DEVELOP_BACKEND_H

#include <QObject>
#include <controller>
#include <glm/vec2.hpp>

class PluginBackend;

class ScanRender:public RenderScript{
public:
    ScanRender();
    glm::vec2 dot_v;
    glm::vec3 orient;
private:
    float percent = 0;
    void scan_line_animation(QTime& t);

    void draw_model(QTime &t);

    friend class PluginBackend;
};
#endif // DEVELOP_BACKEND_H
