#ifndef GL4WINDOW_H
#define GL4WINDOW_H
#include <QtQuick/QQuickWindow>
#include <QSurfaceFormat>

class GL4Window:public QQuickWindow
{
public:
    GL4Window() {
        QSurfaceFormat format;
        // Qt Quick may need a depth and stencil buffer. Always make sure these are available.
        format.setDepthBufferSize(16);
        format.setStencilBufferSize(8);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setRenderableType(QSurfaceFormat::OpenGL);
        format.setMajorVersion(4);
        format.setMinorVersion(1);
        setFormat(format);
    }
};

#endif // GL4WINDOW_H
