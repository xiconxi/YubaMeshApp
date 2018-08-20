#ifndef IPLUGINBACKEND_H
#define IPLUGINBACKEND_H

#include <QObject>
#include <qqml.h>
#include "./ISingleton.inc"

class LIBSHARED_EXPORT IPluginBackend:public QObject
{
    Q_OBJECT
public:

     explicit IPluginBackend(QObject* parent=nullptr):QObject(parent){}

     virtual ~IPluginBackend(){}

     Q_INVOKABLE bool importMesh(QString url);

     virtual bool importMesh(std::string url,std::string name);

     Q_INVOKABLE virtual void construction();

     Q_INVOKABLE virtual void destruction();

private:
    friend class ICtrl<IPluginBackend>;
};

template <typename T>
LIBSHARED_EXPORT QObject* Backend(QQmlEngine *pQMLEngine=nullptr,QJSEngine *pJSEngine=nullptr);

#endif // IPLUGINBACKEND_H
