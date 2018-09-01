TARGET = yubaMs
QT += quick
CONFIG += c++11
QMAKE_RPATHDIR += @executable_path/../Frameworks

TARGET = $$qtLibraryTarget($$TARGET)
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = ../
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp
RESOURCES += qml/qml.qrc
INCLUDEPATH += ../depends
#LIBS += -L$$OUT_PWD/../easylog/ -leasylog

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# Copy the qmldir file to the same folder as the plugin binary
cpPluginsConfig.files += ../plugins/pluginsConfig.xml
cpPluginsConfig.path = $$DESTDIR

cpmeshes.files += mesh/*
cpmeshes.path = $$DESTDIR/Mesh

COPIES += cpPluginsConfig \
            cpmeshes

HEADERS += \
    gl4window.h
ICON = yuba.icns

osx {
    CONFIG(release,debug|release){
        suffix =
    }CONFIG(debug,debug|release){
        suffix = _debug
    }
    for(libname, $$list(GLViewer  MeshDevelop)) { # Measurement
        qtmodules.files += $$OUT_PWD/../modules/$${libname}$${suffix}
    }
    qtmodules.files += $$PWD/../plugins/pluginsConfig.xml
    qtmodules.path = Contents/PlugIns
    QMAKE_BUNDLE_DATA += qtmodules

    meshes.files += $$PWD/Mesh
    meshes.path = Contents
    QMAKE_BUNDLE_DATA += meshes

    depends.files += $$OUT_PWD/../depends/FrameWorks
    depends.path = Contents
    QMAKE_BUNDLE_DATA += depends
}
