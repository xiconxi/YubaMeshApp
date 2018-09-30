#ifndef COMPONENTSRENDER_H
#define COMPONENTSRENDER_H


#include <QObject>
#include <YbCore/controller>
#include <glm/vec2.hpp>

class PluginBackend;

class ComponentsRender:public RenderScript,public IDrawObject
{
public:
    ComponentsRender(TriMesh&& vmesh,int components);

    // GL functions
    void syncVertexBuffersDataScript() override;

    std::vector<int> vertex_cluster;

private:
    float percent = 0;
    void draw(QTime& t);
    void componetAnalysic();

    friend class PluginBackend;
};

#endif // COMPONENTSRENDER_H
