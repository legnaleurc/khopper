DEPENDPATH += . \
              src \
              include/info \
              include/muxer \
              include/ui \
              include/util \
              src/info \
              src/muxer \
              src/ui \
              src/util
INCLUDEPATH += . \
               include/ui \
               include/info \
               include/util \
               include/muxer

# Input
HEADERS += include/info/cuesheet.hpp \
           include/info/index.hpp \
           include/info/track.hpp \
           include/muxer/decoder.hpp \
           include/muxer/encoder.hpp \
           include/ui/mainwindow.hpp \
           include/ui/songlist.hpp \
           include/ui/textcodec.hpp \
           include/ui/threads.hpp \
           include/util/error.hpp \
           include/util/os.hpp \
           include/util/tr1.hpp
SOURCES += src/khopper.cpp \
           src/info/cuesheet.cpp \
           src/info/index.cpp \
           src/info/track.cpp \
           src/muxer/decoder.cpp \
           src/muxer/encoder.cpp \
           src/ui/mainwindow.cpp \
           src/ui/songlist.cpp \
           src/ui/textcodec.cpp \
           src/ui/threads.cpp

# config
TARGET = khopper
MOC_DIR = moc
DESTDIR = bin
CONFIG += debug_and_release

CONFIG( debug, debug|release ) {
    OBJECTS_DIR = obj/debug
} else {
    OBJECTS_DIR = obj/release
    DEFINES += QT_NO_DEBUG_OUTPUT
}

unix {
    TEMPLATE = app
    SOURCES += src/util/linux.cpp
    CONFIG += link_pkgconfig
    PKGCONFIG += libavcodec libavformat
    LIBS += -lloki -lboost_regex
}

win32 {
    TEMPLATE = vcapp
    SOURCES += src/util/windows.cpp
    debug {
        LIBS += -lloki_D
    }
    release {
        LIBS += -lloki
    }
}
