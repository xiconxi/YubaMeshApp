#ifndef CentralCtrl_H
#define CentralCtrl_H
#include <map>
#include <string>
#include <functional>
#include <QOpenGLFunctions_4_1_Core>
#include <easylogging++.h>
#include <QQuickFramebufferObject>
#include "../bases/ISingleton.inc"

typedef QOpenGLFunctions_4_1_Core ActiveGL;
LIBSHARED_EXPORT extern  ActiveGL gl;

class LIBSHARED_EXPORT CentralCtrl
{
public:
    void setFboNode(const QQuickFramebufferObject* ref){fboNode=ref;}

    const QQuickFramebufferObject* GLNode(){return fboNode;}

    void initialGLFunctions();

private:
    CentralCtrl(){}
    friend class ICtrl<CentralCtrl>;
    ~CentralCtrl() = delete;

    const QQuickFramebufferObject* fboNode = nullptr;
};



#endif // CentralCtrl_H
