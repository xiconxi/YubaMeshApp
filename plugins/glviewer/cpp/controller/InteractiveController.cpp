#include "InteractiveController.h"
#include <string>
#include "../utils/Select.h"
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

void InteractiveCtrl::addInteractiveObject(std::string name,InteractiveObject* object){
    if(name2id.count(name) == 0){
        name2id[name] = queryInteractiveId();
        objects[name2id[name]] = object;
    } else {
        // conflit
    }
    focus_object = object;
}

void InteractiveCtrl::delInteractiveObject(InteractiveObject* object){

}

InteractiveObject* InteractiveCtrl::object(std::string name){
    return object(name2id[name]);
}

InteractiveObject* InteractiveCtrl::object(uint id){
    return objects[id];
}

void InteractiveCtrl::focus(InteractiveObject* object) {
    focus_object = object;
}

void InteractiveCtrl::focus(uint id) {
    focus_object = objects[id];
}

InteractiveObject* InteractiveCtrl::focus() {
    return focus_object;
}

std::map<uint,InteractiveObject*>& InteractiveCtrl::allObjects() {
    return objects;
}
