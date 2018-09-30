#ifndef PLUGINBACKEND_H
#define PLUGINBACKEND_H

#include <QObject>
#include <QLine>
#include <QPoint>
#include "SlicesCanvas.h"
#include <glm/vec3.hpp>
#include <YbCore/controller>
#include <complex>
class PickableMesh;
class SliceRender;

class PluginBackend:public IPluginBackend
{
    Q_OBJECT
public:
    explicit PluginBackend(IPluginBackend* parent = nullptr);

    Q_INVOKABLE void slice(int x, int y);

    Q_INVOKABLE void workMode(QString mode);

    float slice(float dis);

    bool importMesh(std::string url,std::string name) override;

    Q_INVOKABLE void construction() override;

    Q_INVOKABLE void setSliceCanvas(QQuickPaintedItem* item);

    Q_INVOKABLE float sliceInRatio(float ratio);

    std::vector<std::vector<QPoint>> contours;

    SlicesCanvas* canvas;

    float gap = 0;

    glm::vec3 norm;

signals:
    void sliceUpdate(QString sliceValue);
    void measureValueUpdate(int valueId, QString ms_value);
private:
    void test(std::vector<std::complex<float> >&  timevec);
    SliceRender* render_s = nullptr;
};

/*
#ifndef PLUGINBACKEND_CPP
extern template QObject* Backend<PluginBackend>(QQmlEngine *pQMLEngine=nullptr,QJSEngine *pJSEngine=nullptr);
#endif
*/
#endif // PLUGINBACKEND_H
