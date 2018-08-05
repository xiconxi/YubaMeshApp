TEMPLATE = lib
TARGET = MeshMorph
QT += qml quick
CONFIG += plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)

# Input
SOURCES += \
    morph_plugin.cpp
HEADERS += \
    morph_plugin.h

CONFIG(release, debug|release): {
    DESTDIR = ../../bin/$$TARGET
    RCC_DIR = ../tmp/$$TARGET
    MOC_DIR = ../tmp/$$TARGET
    OBJECTS_DIR = ../tmp/$$TARGET


    # Copy the qmldir file to the same folder as the plugin binary
    cpqmldir.files += qml/qmldir
    cpqmldir.files += qml/Tools.qml
    cpqmldir.path = $$DESTDIR


    COPIES += cpqmldir

}
