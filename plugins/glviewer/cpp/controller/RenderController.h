#ifndef RenderCtrl_H
#define RenderCtrl_H

#include "../bases/ISingleton.inc"
#include <QtQuick/QQuickFramebufferObject>
#include <functional>
#include <QReadWriteLock>
#include <list>
#include "CentralController.h"

#include "../bases/IRenderScript.h"

class RenderScript;

class LIBSHARED_EXPORT RenderCtrl: public QQuickFramebufferObject::Renderer{

    void render() override ;

    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;

public:

    void update(bool for_next_frame=false);

    void releaseScript(std::string name);

    const QOpenGLFramebufferObject& frameBufferObject() const;

    QReadWriteLock render_lock;

private:

    void addScript(RenderScript&& op);

    void addScript(RenderScript* op);

    RenderCtrl();

    ~RenderCtrl(){}

    friend class ICtrl<RenderCtrl>;
    friend RenderScript::RenderScript(RenderFunc f);
//    friend RenderScript::RenderScript(std::string render_name, RenderFunc f);
    template <typename T> friend class CriticalProperty;

    std::list<std::function<void(QSize)>> size_changed_caller;

    std::list<RenderScript> anonymous_script;
    std::map<std::string,RenderScript*> named_script;

    // 使用两个锁从而支持在script创建script
    QReadWriteLock named_lock;
    QReadWriteLock anonymouse_lock;

    volatile bool for_next_frame = false;
    volatile bool render_volatile = false;

    QOpenGLFramebufferObject* frame_buffer;
};

template <typename T>
class  CriticalProperty{
public:
    void set(T&& value){
        global::con<RenderCtrl>().render_lock.lockForRead();
        property = value;
        global::con<RenderCtrl>().render_lock.unlock();
    }
    void set(T& value){
        set(std::move(value));
    }
    T& value(){
        return property;
    }
private:
    T property;
};

#endif // RenderCtrl_H
