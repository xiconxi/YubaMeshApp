#ifndef MESHCtrl_H
#define MESHCtrl_H

#include "../bases/ISingleton.inc"
#include "CentralController.h"
#include "../mesh/PickableMesh.h"


class LIBSHARED_EXPORT MeshCtrl
{
public:
    PickableMesh* mesh(std::string name);
    PickableMesh* mesh(uint id);

    void addMesh(std::string name, PickableMesh* mesh);

    void releaseMesh(std::string name);

    const std::map<std::string,PickableMesh*>& allMesh(){
        return meshes;
    }

    void setSelectableMesh(uint id);
    void setSelectableMesh(PickableMesh* mesh);
    void setSelectableMesh(std::string name);
    PickableMesh* selectable();

private:
    friend class ICtrl<MeshCtrl>;
    std::map<std::string,PickableMesh*> meshes;
    PickableMesh* selectable_mesh = nullptr;
    MeshCtrl() {}
    ~MeshCtrl() {}
};
#endif // MESHCtrl_H
