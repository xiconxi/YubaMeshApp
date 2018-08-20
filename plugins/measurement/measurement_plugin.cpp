#include "measurement_plugin.h"
#include <qqml.h>
#include "SlicesCanvas.h"
#include "PluginBackend.h"

void MeasurementPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterSingletonType<PluginBackend>(uri, 1, 0, "SubBackends", Backend<PluginBackend>);
    qmlRegisterType<SlicesCanvas>(uri, 1, 0, "SliceCanvas");
}
