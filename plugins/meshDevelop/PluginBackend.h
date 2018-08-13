#ifndef PLUGINBACKEND_H
#define PLUGINBACKEND_H

#include <YbCore/controller>

class ScanRender;
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
