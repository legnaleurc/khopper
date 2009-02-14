DEPENDPATH += . \
              include/album \
              include/codec \
              include/common \
              include/options \
              include/widgets \
              src \
              src/album \
              src/codec \
              src/common \
              src/options \
              src/widgets
INCLUDEPATH += . \
               include/album \
               include/codec \
               include/common \
               include/options \
               include/widgets

# Input
HEADERS   += include/album/cuesheet.hpp \
             include/album/index.hpp \
             include/album/track.hpp \
             include/codec/codec_base.hpp \
             include/codec/decoder.hpp \
             include/codec/encoder.hpp \
             include/common/error.hpp \
             include/common/os.hpp \
             include/common/tr1.hpp \
             include/options/abstractoption.hpp \
             include/widgets/mainwindow.hpp \
             include/widgets/preference.hpp \
             include/widgets/progress.hpp \
             include/widgets/songlist.hpp \
             include/widgets/textcodec.hpp \
             include/widgets/threads.hpp
SOURCES   += src/album/cuesheet.cpp \
             src/album/index.cpp \
             src/album/track.cpp \
             src/codec/decoder.cpp \
             src/codec/encoder.cpp \
             src/common/error.cpp \
             src/options/abstractoption.cpp \
             src/widgets/mainwindow.cpp \
             src/widgets/preference.cpp \
             src/widgets/progress.cpp \
             src/widgets/songlist.cpp \
             src/widgets/textcodec.cpp \
             src/widgets/threads.cpp \
             src/khopper.cpp
RESOURCES += main.qrc

# config
TARGET  = khopper
MOC_DIR = ../tmp/moc
RCC_DIR = ../tmp/rcc
CONFIG += debug_and_release

CONFIG( debug, debug|release ) {
    DESTDIR = ../bin/debug
    OBJECTS_DIR = ../tmp/obj/debug
} else {
    DESTDIR = ../bin/release
    OBJECTS_DIR = ../tmp/obj/release
    DEFINES += QT_NO_DEBUG_OUTPUT
}

unix {
    TEMPLATE = app
    SOURCES += src/common/linux.cpp
    QMAKE_CXXFLAGS += -std=c++0x -fPIC
    CONFIG += link_pkgconfig
    PKGCONFIG += libavcodec libavformat
    INCLUDEPATH += /usr/include/ffmpeg/libavcodec /usr/include/ffmpeg/libavformat
    LIBS += -lloki -lboost_regex
}

win32 {
    TEMPLATE = vcapp
    SOURCES += src/common/windows.cpp
    debug {
        LIBS += -lloki_D
    }
    release {
        LIBS += -lloki
    }
    LIBS += -lavcodec-52 -lavformat-52 -lavutil-49
}
