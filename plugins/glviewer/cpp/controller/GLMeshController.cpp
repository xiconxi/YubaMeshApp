#include "GLMeshController.h"
#include <string>
#include "../utils/Select.h"
#include "../bases/GLMeshObject.h"
#include "../render/InteractiveObject.h"

template<>
LIBSHARED_EXPORT GLMeshCtrl& global::con<GLMeshCtrl>(){
    return ICtrl<GLMeshCtrl>::getGrobalInstanceRef();
}

template<>
LIBSHARED_EXPORT GLMeshCtrl& plugin::con<GLMeshCtrl>(){
    return ICtrl<GLMeshCtrl>::getPluginInstanceRef();
}

GLMeshCtrl::GLMeshCtrl()
{

}

GLMeshCtrl::~GLMeshCtrl()
{
    for(auto& e:objects){
        delete e.second;
    }
}

uint GLMeshCtrl::queryInteractiveId() {
    return interactive_cnts++;
}

void GLMeshCtrl::addInteractiveObject(std::string name,IGLMeshObject* object){
    if(name2id.count(name) == 0){
        name2id[name] = queryInteractiveId();
        objects[name2id[name]] = object;
    } else {
        // conflit
    }
    if(dynamic_cast<InteractiveObject*>(object) != nullptr)
        focus_object = dynamic_cast<InteractiveObject*>(object);
}

void GLMeshCtrl::delInteractiveObject(IGLMeshObject* object){

}

IGLMeshObject* GLMeshCtrl::object(std::string name){
    return object(name2id[name]);
}

IGLMeshObject* GLMeshCtrl::object(uint id){
    return objects[id];
}

InteractiveObject* GLMeshCtrl::interactiveObject(std::string name) {
    return interactiveObject(name2id[name]);
}

InteractiveObject* GLMeshCtrl::interactiveObject(uint id) {
    return dynamic_cast<InteractiveObject*>(object(id));
}

void GLMeshCtrl::focus(InteractiveObject* object) {
    focus_object = object;
}

void GLMeshCtrl::focus(uint id) {
    focus_object = dynamic_cast<InteractiveObject*>(objects[id]);
}

InteractiveObject* GLMeshCtrl::focus() {
    return focus_object;
}

std::map<uint,IGLMeshObject*>& GLMeshCtrl::allObjects() {
    return objects;
}
