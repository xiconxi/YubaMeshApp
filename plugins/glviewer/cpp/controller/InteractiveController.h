#ifndef INTERACTIVECONTROLLER_H
#define INTERACTIVECONTROLLER_H

#include "../bases/ISingleton.inc"
#include <vector>
#include <map>
#include <glm/vec3.hpp>

class PickTool;
class SelectTool;
class InteractiveObject;
class IDrawObject;

class LIBSHARED_EXPORT InteractiveCtrl
{
public:
    InteractiveCtrl();
    ~InteractiveCtrl();
    SelectTool mutable *selectTool;
    PickTool mutable *pickTool;

    void addInteractiveObject(std::string name,IDrawObject* object);
    void delInteractiveObject(IDrawObject* object);
    IDrawObject* object(std::string name);
    IDrawObject* object(uint id);

    InteractiveObject* interactiveObject(std::string name);
    InteractiveObject* interactiveObject(uint id);

    void focus(InteractiveObject* object);
    void focus(uint id);
    InteractiveObject* focus();
    std::map<uint,IDrawObject*>& allObjects();

private:
    uint queryInteractiveId();
    friend class ICtrl<InteractiveCtrl>;
    uint interactive_cnts = 0;
    std::map<std::string,uint> name2id;
    std::map<uint,IDrawObject* > objects;
    InteractiveObject* focus_object = nullptr;
};

#endif // INTERACTIVECONTROLLER_H
