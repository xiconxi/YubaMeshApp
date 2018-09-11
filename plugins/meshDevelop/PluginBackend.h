#ifndef PLUGINBACKEND_H
#define PLUGINBACKEND_H

#include <YbCore/controller>

class ScanRender;

class VertexClusterMeshObject: public IDrawObject {
public:
    VertexClusterMeshObject(TriMesh vmesh,TriMesh nmesh);
    // GL functions
    void syncVertexBuffersDataScript() override;

    std::vector<int> vertex_cluster;
};

class PluginBackend : public IPluginBackend
{
public:
    PluginBackend();
    Q_INVOKABLE void construction() override;
    Q_INVOKABLE void draw_for(QString value);

    bool importMesh(std::string url,std::string name) override;
private:
    ScanRender* render_s;
};

#endif // PLUGINBACKEND_H
