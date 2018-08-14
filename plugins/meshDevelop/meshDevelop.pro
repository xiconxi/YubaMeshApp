TEMPLATE = lib
TARGET = MeshDevelop
QT += qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN


# Input
SOURCES += \
    develop_plugin.cpp \
    PluginBackend.cpp \
    ScanRender.cpp \
    parameterize/TextureFaceMesh.cpp \
    parameterize/ConnectedComponentAnalysis.cpp

HEADERS += \
    develop_plugin.h \
    svpng.inc \
    PluginBackend.h \
    ScanRender.h \
    parameterize/TextureFaceMesh.h \
    parameterize/ConnectedComponentAnalysis.h

CONFIG(release, debug|release): {
    DESTDIR = ../../bin/$$TARGET
    RCC_DIR = ../tmp/$$TARGET
    MOC_DIR = ../tmp/$$TARGET
    OBJECTS_DIR = ../tmp/$$TARGET

    LIBS +=-L$$OUT_PWD/../../bin/GLViewer -lGLViewer
    INCLUDEPATH += ../glviewer/wrapInclude


    # Copy the qmldir file to the same folder as the plugin binary
    cpqmldir.files += qml/qmldir
    cpqmldir.files += qml/Tools.qml
    cpqmldir.path = $$DESTDIR

    cpshaders.files += shaders/*.glsl
    cpshaders.path = $$DESTDIR/shaders

    COPIES += cpqmldir \
        cpshaders
}
