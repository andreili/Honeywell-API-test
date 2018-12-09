QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += ./include/

DEFINES += QT_DEPRECATED_WARNINGS NT

QMAKE_CXXFLAGS += -Wno-comment -Wno-unknown-pragmas -Wno-unused-function -Wno-sign-compare
QMAKE_LFLAGS += -static-libstdc++ -static
QMAKE_LFLAGS_CONSOLE = -Wl,-subsystem,console

#QMAKE_CXXFLAGS += -finstrument-functions

#QMAKE_LIBDIR += d:/Projects/Honeywell/htest/lib/
#LIBS += d:/Projects/Honeywell/htest/lib/hscsrvapi.lib
LIBS += d:/Projects/Honeywell/htest/shheap.lib

SOURCES += \
        main.cpp \
    hscpoint.cpp \
    hscpoints.cpp \
    log.cpp \
    utils.cpp \
    hscpointparam.cpp \
    gbdata.cpp \
    CmdlineParser.cpp \
    trace.cpp \
    hscserver.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    hscpoint.h \
    hscpoints.h \
    log.h \
    hsc_include.h \
    utils.h \
    hscpointparam.h \
    gbdata.h \
    CmdlineParser.h \
    hscserver.h
