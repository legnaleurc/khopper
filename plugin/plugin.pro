TEMPLATE = lib
CONFIG += plugin
INCLUDEPATH += include \
               ../main/include/common \
               ../main/include/options \
               ../main/include/codec
DEPANDPATH += include \
              src \
              ../main/include/options \
              ../main/src/options \
              ../main/include/codec \
              ../main/src/codec
HEADERS = include/mp3option.hpp \
          ../main/include/options/abstractoption.hpp \
          ../main/include/codec/encoder.hpp
SOURCES = src/mp3option.cpp \
          ../main/src/options/abstractoption.cpp \
          ../main/src/codec/encoder.cpp
TARGET = $$qtLibraryTarget(mp3option)
DESTDIR = ../plugins
OBJECTS_DIR = ../tmp/obj
MOC_DIR = ../tmp/moc

unix {
    QMAKE_CXXFLAGS += -std=c++0x
    CONFIG += link_pkgconfig
    PKGCONFIG += libavcodec libavformat
    INCLUDEPATH += /usr/include/ffmpeg/libavcodec /usr/include/ffmpeg/libavformat
}
