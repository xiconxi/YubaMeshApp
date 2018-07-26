#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <easylog/easylogging++.h>
#include "gl4window.h"
#include <QProcess>

//INITIALIZE_EASYLOGGINGPP

int main(int argc, char *argv[])
{    
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

#ifdef Q_OS_OSX
    qmlRegisterType<GL4Window>("YubaCore", 1, 0, "YubaWindow");
    QProcess().startDetached("ln -Fs ../PlugIns/GLViewer/libGLViewer.dylib libGLViewer.dylib");
    engine.addImportPath(app.applicationDirPath() +  "/../PlugIns/");
    engine.rootContext()->setContextProperty("appPath", QGuiApplication::applicationDirPath()+"/../PlugIns/");
    engine.rootContext()->setContextProperty("yubaDoc", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/");
#else
    engine.addImportPath(app.applicationDirPath());
    engine.rootContext()->setContextProperty("appPath", QGuiApplication::applicationDirPath());
#endif
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    auto root = engine.rootContext();
    root->setContextProperty("GMouse", engine.rootObjects().first()->findChild<QObject *>("GLMouseAreaObject"));
    root->setContextProperty("devicePixelRatio", app.devicePixelRatio());
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
