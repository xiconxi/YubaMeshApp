#include "CentralController.h"
#include <QTime>
#include "ViewController.h"
#include "ShaderController.h"
#include "RenderController.h"
#include "GLMeshController.h"

ActiveGL gl;

template<>
LIBSHARED_EXPORT CentralCtrl& global::con<CentralCtrl>(){
    return ICtrl<CentralCtrl>::getGrobalInstanceRef();
}

void CentralCtrl::initialGLFunctions() {
    gl.initializeOpenGLFunctions();
}

