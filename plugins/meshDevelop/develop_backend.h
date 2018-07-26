#ifndef DEVELOP_BACKEND_H
#define DEVELOP_BACKEND_H

#include <QObject>
#include "../glviewer/manager/RenderManager.h"
#include <glm/vec2.hpp>

class develop_backend;

class A:public RenderScript{
public:
    A();
    glm::vec2 dot_v;
private:
    float percent = 0;
    void scan_line_animation(RenderScript *f);

    void draw_model(RenderScript *f);

    friend class develop_backend;
};

class develop_backend:public QObject
{
    Q_OBJECT
public:
    develop_backend(QObject* parent=nullptr);

    Q_INVOKABLE void glinit();

    Q_INVOKABLE void draw_for(QString value);
private:
    void initial_gl_resource();
    A* render_a = nullptr;
};

#endif // DEVELOP_BACKEND_H
