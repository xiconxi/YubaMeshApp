TEMPLATE = lib

INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN

HEADERS += \
    inc/YbMesh/YbMesh.hpp \
    src/meshcodecs/format_obj.h \
    src/type_mesh.h

SOURCES += \
    ../easyloggingpp/src/easylogging++.cc \
    src/meshcodecs/format_obj.cpp

TARGET = YbMesh
DESTDIR = ../FrameWorks/

QMAKE_LFLAGS_SONAME = '-Wl,-install_name,@rpath/'
