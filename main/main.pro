DEPENDPATH += . \
              include/album \
              include/codec \
              include/common \
              include/plugin \
              include/widget \
              src \
              src/album \
              src/codec \
              src/common \
              src/plugin \
              src/widget
INCLUDEPATH += . \
               include/album \
               include/codec \
               include/common \
               include/plugin \
               include/widget

# Input
HEADERS   += include/album/cuesheet.hpp \
             include/album/index.hpp \
             include/album/track.hpp \
             include/common/error.hpp \
             include/common/os.hpp \
             include/common/text.hpp \
             include/common/tr1.hpp \
             include/plugin/codec_base.hpp \
             include/plugin/abstractaudioreader.hpp \
             include/plugin/abstractaudiowriter.hpp \
             include/codec/defaultaudioreader.hpp \
             include/codec/defaultaudiowriter.hpp \
             include/plugin/abstractpanel.hpp \
             include/widget/converter.hpp \
             include/widget/mainwindow.hpp \
             include/widget/preference.hpp \
             include/widget/progress.hpp \
             include/widget/songlist.hpp \
             include/widget/textcodec.hpp \
             include/widget/threads.hpp
SOURCES   += src/album/cuesheet.cpp \
             src/album/index.cpp \
             src/album/track.cpp \
             src/common/error.cpp \
             src/common/text.cpp \
             src/plugin/abstractaudioreader.cpp \
             src/plugin/abstractaudiowriter.cpp \
             src/codec/defaultaudioreader.cpp \
             src/codec/defaultaudiowriter.cpp \
             src/plugin/abstractpanel.cpp \
             src/widget/converter.cpp \
             src/widget/mainwindow.cpp \
             src/widget/preference.cpp \
             src/widget/progress.cpp \
             src/widget/songlist.cpp \
             src/widget/textcodec.cpp \
             src/widget/threads.cpp \
             src/main.cpp
RESOURCES += main.qrc

# config
TARGET  = khopper
MOC_DIR = ../tmp/moc
RCC_DIR = ../tmp/rcc
CONFIG += debug_and_release
QT     += phonon

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
    LIBS += -lboost_regex
}

win32 {
    TEMPLATE = vcapp
    SOURCES += src/common/windows.cpp
    LIBS += -lavcodec-52 -lavformat-52 -lavutil-49
}
