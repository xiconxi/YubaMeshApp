TEMPLATE = lib
TARGET = easylog


TARGET = $$qtLibraryTarget($$TARGET)
# Input
SOURCES += \
        easylogging++.cc


HEADERS += \
        easylogging++.h


DESTDIR = ../../$$TARGET

