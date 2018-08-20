#include "SlicesCanvas.h"
#include "PluginBackend.h"
#include <easylogging++.h>
#include <QPen>
SlicesCanvas::SlicesCanvas() {

}

void SlicesCanvas::paint(QPainter *painter) {
    QPen pen(QColor(0, 0, 255, 80));
    pen.setWidth(1);
    painter->setPen(pen);

    QBrush brush(QColor(255, 0, 0, 80));
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);

    for(auto e:((PluginBackend*)Backend<PluginBackend>())->contours)
        painter->drawPolygon(e.data(), e.size());
}
