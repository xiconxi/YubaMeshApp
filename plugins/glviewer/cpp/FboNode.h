#ifndef FBO_RENDERER_H
#define FBO_RENDERER_H

#include <QtQuick/QQuickFramebufferObject>
#include <QtQuick/QQuickPaintedItem>

class FboNode:public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(qreal fps MEMBER m_fps NOTIFY fpsChanged)
public:
    FboNode();

    QQuickFramebufferObject::Renderer *createRenderer() const ;

    void set_fps (qreal time) const;

signals:
    void fpsChanged();

public slots:
    void move(int x, int y);
    void rotate(int x, int y);
    void scaleBy(float s);

    void singleFacePick(int x,int y);
    void screenAreaPick(QQuickPaintedItem* provider);

    void registerModule(QString module);
    void unregisterModule(QString module);

    void axesVisible(bool status);
    void boxVisible(bool status);
private:
    qreal mutable m_fps = 60;
};

#define FNode FboNode

#endif // FBO_RENDERER_H
