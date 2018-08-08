#ifndef PLUGINBACKEND_H
#define PLUGINBACKEND_H

#include <controller>

class ScanRender;
class PluginBackend : public IPluginBackend
{
public:
    PluginBackend();
    Q_INVOKABLE void construction() override;
    Q_INVOKABLE void draw_for(QString value);
private:
    ScanRender* render_s;
};

#endif // PLUGINBACKEND_H
