#include "develop_plugin.h"
#include "develop_backend.h"

#include <qqml.h>

#include "../glviewer/manager/RenderManager.h"
#include "../glviewer/manager/ShaderManager.h"
#include "../glviewer/manager/MeshManager.h"
#include "../glviewer/manager/ViewManager.h"
#include "../glviewer/mesh/meshCodes.h"
#include "../glviewer/mesh/meshProcessing.h"

#include "algorithm/meshProcessing.h"


static QObject *subBackendFunc(QQmlEngine *pQMLEngine,QJSEngine *pJSEngine) {
    Q_UNUSED(pQMLEngine);
    Q_UNUSED(pJSEngine);
    static develop_backend* ptr = nullptr;
    if( ptr == nullptr){
#if defined(Q_OS_OSX)
        QString prefix = "../PlugIns/MeshDevelop/shaders/";
#elif defined(Q_OS_WIN)
        QString prefix = "./MeshDevelop/shaders/";
#endif
        RenderScript([=](RenderScript* rf) mutable {
            SM->addShaderProgram("texture", shaderConfig{ V(prefix+"texture"),G(prefix+"texture"),F(prefix+"texture") });
            SM->addShaderProgram("base", shaderConfig{ V(prefix+"indices"),F(prefix+"indices") });
            glFormatMesh mesh;
            readObj("../PlugIns/GLViewer/mesh/FullBodyScan.obj", &mesh);
            calculateNorm(&mesh);
            centerlized(&mesh);
            sortByVector(&mesh, glm::normalize(glm::mat3(glm::inverse(mesh.model))*glm::vec3(0,1,0)));
            TM->add_mesh("bunny",new RenderMesh(std::move(mesh))); //bunny FullBodyScan 20180205142827.cie
        });
        ptr = new develop_backend();
    }
    return ptr;
}

void MorphPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterSingletonType<develop_backend>(uri, 1, 0, "SubBackends", subBackendFunc);
}
