#include "develop_plugin.h"
#include "develop_backend.h"

#include <qqml.h>
#include <controller>
#include <mesh_proc>

static QObject *subBackendFunc(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    static develop_backend* ptr = nullptr;
    if( ptr == nullptr){
        ptr = new develop_backend();
    }
    return ptr;
}

void MorphPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterSingletonType<develop_backend>(uri, 1, 0, "SubBackends", subBackendFunc);
}
