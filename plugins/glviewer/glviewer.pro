TEMPLATE = lib
TARGET = GLViewer
QT += qml quick
CONFIG += plugin
DEFINES += GLVIEWER_LIBRARY
TARGET = $$qtLibraryTarget($$TARGET)
INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN \
    ../../depends/YbMesh/inc \

# Input
SOURCES += \
    ../../depends/easyloggingpp/src/easylogging++.cc \
    cpp/FboNode.cpp \
    cpp/glviewer_plugin.cpp \
    cpp/controller/CentralController.cpp \
#    cpp/controller/MeshController.cpp \
    cpp/controller/RenderController.cpp \
    cpp/controller/ShaderController.cpp \
    cpp/controller/ViewController.cpp \
    cpp/utils/Select.cpp \
    cpp/utils/SelectCanvas.cpp \
    cpp/utils/Pick.cpp \
    cpp/utils/ScriptSamples.cpp \
    cpp/bases/IPluginBackend.cpp \
    cpp/bases/IRenderScript.cpp \
    cpp/utils/auxiliary.cpp \
    cpp/bases/GLMeshObject.cpp \
    cpp/controller/GLMeshController.cpp \
    ../glviewer/cpp/render/InteractiveObject.cpp \
    cpp/render/InteractiveFaceTexObject.cpp




HEADERS += \
    cpp/FboNode.h \
    cpp/glviewer_plugin.h \
    cpp/bases/glviewerGlobal.inc \
    cpp/bases/IPluginBackend.h \
    cpp/bases/ISingleton.inc \
    cpp/controller/CentralController.h \
    cpp/controller/RenderController.h \
    cpp/controller/ShaderController.h \
    cpp/controller/ViewController.h \
    cpp/utils/Select.h \
    cpp/utils/Pick.h \
    cpp/utils/SelectCanvas.h \
    cpp/utils/ScriptSamples.h \
    cpp/bases/IRenderScript.h \
    wrapInclude/YbCore/controller \
    wrapInclude/YbCore/coretool \
    wrapInclude/YbCore/scripts \
    cpp/utils/auxiliary.h \
    cpp/bases/GLMeshObject.h \
    cpp/controller/GLMeshController.h \
    ../glviewer/cpp/render/InteractiveObject.h \
    cpp/render/InteractiveFaceTexObject.h



DESTDIR = ../../modules/$$TARGET
RCC_DIR = ../tmp/$$TARGET
MOC_DIR = ../tmp/$$TARGET
OBJECTS_DIR = ../tmp/$$TARGET
# Copy the qmldir file to the same folder as the plugin binary
cpqmldir.files += qml/qmldir \
    qml/*.qml
cpqmldir.path = $$DESTDIR

cpshaders.files += glsl/*.glsl
cpshaders.path = $$DESTDIR/glsl

COPIES += cpqmldir \
    cpshaders

INCLUDEPATH += ../../depends/YbMesh/inc
LIBS +=-L$$OUT_PWD/../../depends/FrameWorks/ -lYbMesh

osx{
    QMAKE_POST_LINK += install_name_tool -id @rpath/libGLViewer.dylib ../../modules/$$TARGET/libGLViewer.dylib
}

DISTFILES += \
    glsl/select__mask_adjacency.g.glsl
