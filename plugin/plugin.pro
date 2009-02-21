TEMPLATE = lib
CONFIG += plugin debug_and_release
INCLUDEPATH += include \
               ../main/include/common \
               ../main/include/plugin \
               ../main/include/codec
DEPANDPATH += include \
              src \
              ../main/include/plugin \
              ../main/include/common \
              ../main/src/common \
              ../main/src/plugin \
              ../main/include/codec \
              ../main/src/codec
HEADERS = include/mp3panel.hpp \
          ../main/include/plugin/abstractpanel.hpp \
          ../main/include/plugin/abstractaudiowriter.hpp \
          ../main/include/codec/defaultaudiowriter.hpp
SOURCES = src/mp3panel.cpp \
          ../main/src/plugin/abstractpanel.cpp \
          ../main/src/plugin/abstractaudiowriter.cpp \
          ../main/src/codec/defaultaudiowriter.cpp \
          ../main/src/common/error.cpp
TARGET = $$qtLibraryTarget(khopper_panel_mp3)
MOC_DIR = ../tmp/moc

CONFIG( debug, debug|release ) {
    DESTDIR = ../bin/debug/plugins
    OBJECTS_DIR = ../tmp/obj/debug
} else {
    DESTDIR = ../bin/release/plugins
    OBJECTS_DIR = ../tmp/obj/release
    DEFINES += QT_NO_DEBUG_OUTPUT
}

unix {
    QMAKE_CXXFLAGS += -std=c++0x
    CONFIG += link_pkgconfig
    PKGCONFIG += libavcodec libavformat
    INCLUDEPATH += /usr/include/ffmpeg/libavcodec /usr/include/ffmpeg/libavformat
}
