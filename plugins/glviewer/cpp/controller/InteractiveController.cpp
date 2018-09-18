#include "InteractiveController.h"
#include <string>
#include "../utils/Select.h"
#include "../bases/InteractiveObjectMesh.h"

template<>
LIBSHARED_EXPORT InteractiveCtrl& con<InteractiveCtrl>(){
    return ICtrl<InteractiveCtrl>::getInstanceRef();
}

InteractiveCtrl::InteractiveCtrl()
{

}

uint InteractiveCtrl::queryInteractiveId() {
    return interactive_cnts++;
}

void InteractiveCtrl::addInteractiveObject(std::string name,IDrawObject* object){
    if(name2id.count(name) == 0){
        name2id[name] = queryInteractiveId();
        objects[name2id[name]] = object;
    } else {
        // conflit
    }
    if(dynamic_cast<InteractiveObject*>(object) != nullptr)
        focus_object = dynamic_cast<InteractiveObject*>(object);
}

void InteractiveCtrl::delInteractiveObject(IDrawObject* object){

}

IDrawObject* InteractiveCtrl::object(std::string name){
    return object(name2id[name]);
}

IDrawObject* InteractiveCtrl::object(uint id){
    return objects[id];
}

InteractiveObject* InteractiveCtrl::interactiveObject(std::string name) {
    return interactiveObject(name2id[name]);
}

InteractiveObject* InteractiveCtrl::interactiveObject(uint id) {
    return dynamic_cast<InteractiveObject*>(object(id));
}

void InteractiveCtrl::focus(InteractiveObject* object) {
    focus_object = object;
}

void InteractiveCtrl::focus(uint id) {
    focus_object = dynamic_cast<InteractiveObject*>(objects[id]);
}

InteractiveObject* InteractiveCtrl::focus() {
    return focus_object;
}

std::map<uint,IDrawObject*>& InteractiveCtrl::allObjects() {
    return objects;
}
