#ifndef WHIRLWINDRENDER_H
#define WHIRLWINDRENDER_H
#include <QObject>
#include <YbCore/controller>
#include <glm/vec2.hpp>

class PluginBackend;

class WhirlwindRender:public IDrawObject,public RenderScript
{
public:
    WhirlwindRender(TriMesh&& mesh);

private:
    void draw(QTime& t);

    friend class PluginBackend;
};

// algorithm
void invincibleWhirlwindTriangle(YbMesh::indicesTriMesh<glm::vec3>& mesh);

#endif // WHIRLWINDRENDER_H
