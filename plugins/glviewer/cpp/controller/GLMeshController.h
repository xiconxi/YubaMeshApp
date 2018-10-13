#ifndef INTERACTIVECONTROLLER_H
#define INTERACTIVECONTROLLER_H

#include "../bases/ISingleton.inc"
#include <vector>
#include <map>
#include <glm/vec3.hpp>

class PickTool;
class SelectTool;
class InteractiveObject;
class IGLMeshObject;

class LIBSHARED_EXPORT GLMeshCtrl
{
public:
    GLMeshCtrl();
    ~GLMeshCtrl();
    SelectTool mutable *selectTool;
    PickTool mutable *pickTool;

    void addInteractiveObject(std::string name,IGLMeshObject* object);
    void delInteractiveObject(IGLMeshObject* object);
    IGLMeshObject* object(std::string name);
    IGLMeshObject* object(uint id);

    InteractiveObject* interactiveObject(std::string name);
    InteractiveObject* interactiveObject(uint id);

    void focus(InteractiveObject* object);
    void focus(uint id);
    InteractiveObject* focus();
    std::map<uint,IGLMeshObject*>& allObjects();

private:
    uint queryInteractiveId();
    friend class ICtrl<GLMeshCtrl>;
    uint interactive_cnts = 0;
    std::map<std::string,uint> name2id;
    std::map<uint,IGLMeshObject* > objects;
    InteractiveObject* focus_object = nullptr;
};

#endif // INTERACTIVECONTROLLER_H
