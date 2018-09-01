#ifndef INTERACTIVECONTROLLER_H
#define INTERACTIVECONTROLLER_H

#include "../bases/ISingleton.inc"
#include <vector>
#include <map>

class PickTool;
class SelectTool;
class InteractiveObject;

class InteractiveCtrl
{
public:
    InteractiveCtrl();
    SelectTool mutable *selectTool;
    PickTool mutable *pickTool;

    void addInteractiveObject(std::string name,InteractiveObject* object);
    void delInteractiveObject(InteractiveObject* object);
    InteractiveObject* object(std::string name);
    InteractiveObject* object(uint id);

    void focus(InteractiveObject* object);
    void focus(uint id);
    InteractiveObject* focus();
    std::map<uint,InteractiveObject*>& allObjects();

private:
    uint queryInteractiveId();
    friend class ICtrl<InteractiveCtrl>;
    uint interactive_cnts = 0;
    std::map<std::string,uint> name2id;
    std::map<uint,InteractiveObject*> objects;
    InteractiveObject* focus_object = nullptr;
};

#endif // INTERACTIVECONTROLLER_H
