#ifndef SELECTCANVAS_H
#define SELECTCANVAS_H

#include <QQuickItem>
#include <QPainter>
#include <QPainterPath>
#include <QQuickPaintedItem>
#include <vector>
#include <QPointF>
#include <QtOpenGL/QGL>

class SelectCanvas : public QQuickPaintedItem
{
    Q_OBJECT
public:
    SelectCanvas();
    void paint(QPainter *painter) override;
public slots:
    void moveTo(qreal x, qreal y);
    void lineTo(qreal x, qreal y);
    void clear();
signals:

private:
    std::vector<QPointF> pathes;
};


#endif // SELECTCANVAS_H
