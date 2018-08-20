#ifndef IRENDERSCRIPT_H
#define IRENDERSCRIPT_H

#include "glviewerGlobal.inc"
#include <functional>
#include <string>

class RenderCtrl;
class QTime;

typedef std::function<void(QTime&)> RenderFunc;
class LIBSHARED_EXPORT RenderScript
{
    std::string name;
public:
    RenderScript(RenderFunc f);
    virtual ~RenderScript(){}
    static void* operator new(size_t size, std::string render_name);
    void changeRender(RenderFunc&& f);
protected:
#ifdef GL_ERROR_HOOK
    void render(QTime& t);

    RenderFunc m_render;
#else
    RenderFunc render;
#endif
    static std::string static_name;
    friend class RenderCtrl;
};

#define AnonymouseScript(m_f) RenderScript(std::bind(&m_f, this, std::placeholders::_1))
#endif // IRENDERSCRIPT_H
