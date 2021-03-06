#ifndef PLUGINBACKEND_H
#define PLUGINBACKEND_H

#include <YbCore/controller>

class ComponentsRender;
class WhirlwindRender;
class GridTextureRender;
class PerFaceTexRender;

class PluginBackend : public IPluginBackend
{
    Q_OBJECT
public:
    PluginBackend();
    Q_INVOKABLE void construction() override;


    Q_INVOKABLE void checkComponents(int id, bool status);

//    Q_INVOKABLE void initialComponentsRender();

    bool importMesh(std::string url,std::string name) override;
private:
    ComponentsRender* components_render;
    WhirlwindRender* whirlwind_render;
    GridTextureRender* gridtexture_render;
    PerFaceTexRender* perfacetex_render;
};

#endif // PLUGINBACKEND_H
