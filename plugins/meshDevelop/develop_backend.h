#ifndef DEVELOP_BACKEND_H
#define DEVELOP_BACKEND_H

#include <QObject>
#include <controller>
#include <glm/vec2.hpp>

class develop_backend;

class A:public RenderScript{
public:
    A();
    glm::vec2 dot_v;
private:
    float percent = 0;
    void scan_line_animation(QTime& t);

    void draw_model(QTime &t);

    friend class develop_backend;
};

class develop_backend:public QObject
{
    Q_OBJECT
public:
    develop_backend(QObject* parent=nullptr);

    Q_INVOKABLE void draw_for(QString value);

    Q_INVOKABLE void instance();
private:
    A* render_a = nullptr;
};

#endif // DEVELOP_BACKEND_H
