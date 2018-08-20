#ifndef CentralCtrl_H
#define CentralCtrl_H
#include <map>
#include <string>
#include <functional>
#include <QOpenGLFunctions_4_1_Core>
#include <easylogging++.h>
#include <QQuickFramebufferObject>
#include "../bases/ISingleton.inc"
class PickTool;
class SelectTool;

typedef QOpenGLFunctions_4_1_Core ActiveGL;
LIBSHARED_EXPORT extern  ActiveGL gl;

class LIBSHARED_EXPORT CentralCtrl
{
public:
    enum RSTYPE{
        MESH, RENDER, SHADER, VIEW
    };

    void setModuleName(std::string name);

    void releaseModuleResource(std::string name);

    void resourceHook(RSTYPE type, std::string name);

    std::string module() const{return current_module_name;}

    void setFboNode(const QQuickFramebufferObject* ref){fboNode=ref;}

    const QQuickFramebufferObject* GLNode(){return fboNode;}

    void initialGLFunctions();

    SelectTool mutable *selectTool;
    PickTool mutable *pickTool;
private:
    CentralCtrl(){}
    friend class ICtrl<CentralCtrl>;
    ~CentralCtrl() = delete;
    std::string current_module_name = "~";
    std::map<std::string, std::list<std::pair<RSTYPE,std::string>> > resource;

    const QQuickFramebufferObject* fboNode = nullptr;
};



#endif // CentralCtrl_H
