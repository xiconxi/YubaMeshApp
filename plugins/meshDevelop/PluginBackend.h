#ifndef PLUGINBACKEND_H
#define PLUGINBACKEND_H

#include <YbCore/controller>

class ScanRender;

class VertexClusterMeshObject: public IDrawObject {
public:
    VertexClusterMeshObject(TriMesh vmesh,TriMesh nmesh,int components);
    // GL functions
    void syncVertexBuffersDataScript() override;

    std::vector<int> vertex_cluster;
};

class PluginBackend : public IPluginBackend
{
    Q_OBJECT
public:
    PluginBackend();
    Q_INVOKABLE void construction() override;
    Q_INVOKABLE void checkComponents(int id, bool status);
    Q_INVOKABLE void draw_for(QString value);

    bool importMesh(std::string url,std::string name) override;
private:
    ScanRender* render_s;
};

#endif // PLUGINBACKEND_H
