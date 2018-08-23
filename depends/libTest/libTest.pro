TARGET = libTest
QMAKE_RPATHDIR += @executable_path/../Frameworks
CONFIG += c++11

SOURCES += \
    main.cpp

INCLUDEPATH += \
    ../../depends/easyloggingpp/src \
    ../../depends/GLM \
    ../../depends/EIGEN

INCLUDEPATH += ../yubaMesh
LIBS +=-L$$OUT_PWD/../bin/ -lYubaMesh

osx{
   frameworks.files += $$OUT_PWD/../bin/
   frameworks.path = Contents/FrameWorks
   QMAKE_BUNDLE_DATA += frameworks
}
