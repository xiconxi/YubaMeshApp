TEMPLATE = lib
TARGET = Measurement
QT += qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN
# Input
SOURCES += \
    measurement_plugin.cpp \
#    measurement_backend.cpp \
    SlicesCanvas.cpp \
    algorithm/MeshProcessing.cpp \
    PluginBackend.cpp \
    SliceRender.cpp

HEADERS += \
    measurement_plugin.h \
#    measurement_backend.h \
    SlicesCanvas.h \
    algorithm/MeshProcessing.h \
    PluginBackend.h \
    SliceRender.h


TARGET = $$qtLibraryTarget($$TARGET)

CONFIG(release, debug|release): {
    DESTDIR = ../../bin/$$TARGET
    RCC_DIR = ../tmp/$$TARGET
    MOC_DIR = ../tmp/$$TARGET
    OBJECTS_DIR = ../tmp/$$TARGET

    INCLUDEPATH += ../glviewer/wrapInclude
    LIBS +=-L$$OUT_PWD/../../bin/GLViewer -lGLViewer


    # Copy the qmldir file to the same folder as the plugin binary
    cpqmldir.files += qml/qmldir
    cpqmldir.files += qml/Tools.qml
    cpqmldir.path = $$DESTDIR

    cpshaders.files += shaders/*.glsl
    cpshaders.path = $$DESTDIR/shaders

    COPIES += cpqmldir \
        cpshaders
}