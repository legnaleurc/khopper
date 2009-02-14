TEMPLATE = lib
CONFIG += plugin debug_and_release
INCLUDEPATH += include \
               ../main/include/common \
               ../main/include/options \
               ../main/include/codec
DEPANDPATH += include \
              src \
              ../main/include/options \
              ../main/include/common \
              ../main/src/common \
              ../main/src/options \
              ../main/include/codec \
              ../main/src/codec
HEADERS = include/mp3option.hpp \
          ../main/include/options/abstractoption.hpp \
          ../main/include/codec/encoder.hpp
SOURCES = src/mp3option.cpp \
          ../main/src/options/abstractoption.cpp \
          ../main/src/codec/encoder.cpp \
          ../main/src/common/error.cpp
TARGET = $$qtLibraryTarget(mp3option)
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
