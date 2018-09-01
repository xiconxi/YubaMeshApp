#include "develop_plugin.h"
#include "PluginBackend.h"
#include <qqml.h>
#include <YbCore/controller>

void MorphPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterSingletonType<PluginBackend>(uri, 1, 0, "SubBackends", Backend<PluginBackend>);
}
