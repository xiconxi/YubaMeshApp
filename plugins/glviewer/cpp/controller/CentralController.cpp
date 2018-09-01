#include "CentralController.h"
#include <QTime>
#include "ViewController.h"
#include "ShaderController.h"
#include "RenderController.h"

ActiveGL gl;

template<>
LIBSHARED_EXPORT CentralCtrl& con<CentralCtrl>(){
    return ICtrl<CentralCtrl>::getInstanceRef();
}

void CentralCtrl::initialGLFunctions() {
    gl.initializeOpenGLFunctions();
}

void CentralCtrl::setModuleName(std::string name) {
    this->current_module_name = name+"~";
    resource[name] = std::list<std::pair<RSTYPE, std::string>>();
    LOG(INFO) << ">>>> register module : " <<  this->current_module_name;
}

// todo
void CentralCtrl::releaseModuleResource(std::string name) {
    name += "~";
    for(auto& e:resource[name]) {
        switch (e.first) {
        case RENDER:
            con<RenderCtrl>().releaseScript(name+e.second);
            break;
        case SHADER:
            con<ShaderCtrl>().releaseShader(name+e.second);
            break;
        case VIEW:
            con<ViewCtrl>().releaseView(name+e.second);
            break;
        default:
            break;
        }
    }
    LOG(INFO) << "<<<< unregister module : " <<  name ;
}

void CentralCtrl::resourceHook(RSTYPE type, std::string name) {
    resource[current_module_name].emplace_back(std::make_pair(type, name));
}

