#ifndef SLICESCANVAS_H
#define SLICESCANVAS_H
#include <QQuickItem>
#include <QPainter>
#include <QPainterPath>
#include <QQuickPaintedItem>
#include <vector>
#include <QPointF>
#include <QtOpenGL/QGL>

class SlicesCanvas : public QQuickPaintedItem
{
    Q_OBJECT
public:
    SlicesCanvas();
    void paint(QPainter *painter) override;
signals:
    void sliceChanged();
};

#endif // SLICESCANVAS_H
