#include "glviewer_plugin.h"

#include <qqml.h>
#include "FboNode.h"
#include "utils/SelectCanvas.h"
#include <stdint.h>
#include <easylogging++.h>

INITIALIZE_EASYLOGGINGPP

void GlviewerPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<FboNode>(uri, 1, 0, "GlViewer");
    qmlRegisterType<SelectCanvas>(uri, 1, 0, "SelectCanvas");
}

