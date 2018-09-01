TARGET = libTest
QMAKE_RPATHDIR += @executable_path/../Frameworks
CONFIG += c++11

SOURCES += \
    main.cpp

INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN

INCLUDEPATH += ../YbMesh/inc
LIBS +=-L$$OUT_PWD/../FrameWorks/ -lYbMesh

osx{
   frameworks.files += $$OUT_PWD/../FrameWorks
   frameworks.path = Contents
   QMAKE_BUNDLE_DATA += frameworks
}
