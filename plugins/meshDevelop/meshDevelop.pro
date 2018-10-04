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
    render/ComponentsRender.cpp \
    render/WhirlwindRender.cpp \
    render/GridTextureRender.cpp

HEADERS += \
    develop_plugin.h \
    svpng.inc \
    PluginBackend.h \
    render/ComponentsRender.h \
    render/WhirlwindRender.h \
    render/GridTextureRender.h

CONFIG(release, debug|release): {
    DESTDIR = ../../modules/$$TARGET
    RCC_DIR = ../tmp/$$TARGET
    MOC_DIR = ../tmp/$$TARGET
    OBJECTS_DIR = ../tmp/$$TARGET

    INCLUDEPATH += ../glviewer/wrapInclude
    LIBS +=-L$$OUT_PWD/../../modules/GLViewer -lGLViewer

    INCLUDEPATH += ../../depends/YbMesh/inc
    LIBS +=-L$$OUT_PWD/../../depends/FrameWorks/ -lYbMesh

    # Copy the qmldir file to the same folder as the plugin binary
    cpqmldir.files += qml/qmldir
    cpqmldir.files += qml/Tools.qml
    cpqmldir.path = $$DESTDIR

    cpshaders.files += shaders/*.glsl
    cpshaders.path = $$DESTDIR/shaders

    COPIES += cpqmldir \
        cpshaders
}
