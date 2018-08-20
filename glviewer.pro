TEMPLATE = lib
TARGET = GLViewer
QT += qml quick
CONFIG += plugin c++14
DEFINES += GLVIEWER_LIBRARY
TARGET = $$qtLibraryTarget($$TARGET)
INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN

# Input
SOURCES += \
    ../../depends/easyloggingpp/src/easylogging++.cc \
    cpp/FboNode.cpp \
    cpp/glviewer_plugin.cpp \
    cpp/controller/CentralController.cpp \
    cpp/controller/MeshController.cpp \
    cpp/controller/RenderController.cpp \
    cpp/controller/ShaderController.cpp \
    cpp/controller/ViewController.cpp \
    cpp/mesh/meshCodes.cpp \
    cpp/mesh/meshProcessing.cpp \
    cpp/mesh/PickableMesh.cpp \
    cpp/mesh/half_edge.cpp \
    cpp/utils/Select.cpp \
    cpp/utils/SelectCanvas.cpp \
    cpp/utils/Pick.cpp \
    cpp/bases/IPluginBackend.cpp \
    cpp/utils/ScriptSamples.cpp \
    cpp/bases/IRenderScript.cpp


HEADERS += \
    cpp/FboNode.h \
    cpp/glviewer_plugin.h \
    cpp/bases/glviewerGlobal.inc \
    cpp/bases/IPluginBackend.h \
    cpp/bases/ISingleton.inc \
    cpp/controller/CentralController.h \
    cpp/controller/MeshController.h \
    cpp/controller/RenderController.h \
    cpp/controller/ShaderController.h \
    cpp/controller/ViewController.h \
    cpp/mesh/meshCodes.h \
    cpp/mesh/meshProcessing.h \
    cpp/mesh/PickableMesh.h \
    cpp/mesh/half_edge.h \
    cpp/utils/Select.h \
    cpp/utils/Pick.h \
    cpp/utils/SelectCanvas.h \
    cpp/utils/ScriptSamples.h \
    cpp/bases/IRenderScript.h \
    wrapInclude/YbCore/controller \
    wrapInclude/YbCore/coretool \
    wrapInclude/YbCore/mesh_proc \
    wrapInclude/YbCore/scripts

DESTDIR = ../../bin/$$TARGET
RCC_DIR = ../tmp/$$TARGET
MOC_DIR = ../tmp/$$TARGET
OBJECTS_DIR = ../tmp/$$TARGET
# Copy the qmldir file to the same folder as the plugin binary
cpqmldir.files += qml/qmldir \
    qml/*.qml
cpqmldir.path = $$DESTDIR

cpshaders.files += glsl/*.glsl
cpshaders.path = $$DESTDIR/glsl

cpmeshes.files += mesh_data/*
cpmeshes.path = $$DESTDIR/mesh

COPIES += cpqmldir \
    cpshaders \
    cpmeshes
