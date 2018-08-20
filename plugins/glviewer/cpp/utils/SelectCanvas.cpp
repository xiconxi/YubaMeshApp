#include "SelectCanvas.h"
#include <easylogging++.h>
#include <QSGTextureProvider>
#include "../controller/RenderController.h"
#include "../controller/MeshController.h"
#include "../controller/ShaderController.h"
#include "../controller/ViewController.h"

SelectCanvas::SelectCanvas()
{

}

void SelectCanvas::paint(QPainter *painter) {
    QBrush brush(QColor(255, 0, 0, 80));

    painter->setBrush(brush);
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawPolygon(pathes.data(), pathes.size());
}

void SelectCanvas::moveTo(qreal x, qreal y) {
    pathes.clear();
    pathes.push_back(QPointF(x,y));
    this->update();
}

void SelectCanvas::lineTo(qreal x, qreal y) {
    pathes.push_back(QPointF(x,y));
    this->update();
}

void SelectCanvas::clear() {
    pathes.clear();
    this->update();
}
