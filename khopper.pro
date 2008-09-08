DEPENDPATH += . \
              src \
              include/info \
              include/muxer \
              include/ui \
              include/util \
              src/info \
              src/muxer \
              src/ui \
              src/util \
              include/muxer/dec \
              include/muxer/enc \
              src/muxer/dec \
              src/muxer/enc
INCLUDEPATH += . \
               include/ui \
               include/info \
               include/util \
               include/muxer \
               include/muxer/dec \
               include/muxer/enc

# Input
HEADERS += include/info/audiofile.hpp \
           include/info/cuesheet.hpp \
           include/info/track.hpp \
           include/muxer/converter.hpp \
           include/muxer/decoder.hpp \
           include/muxer/encoder.hpp \
           include/ui/mainwindow.hpp \
           include/ui/songlist.hpp \
           include/ui/textcodec.hpp \
           include/ui/threads.hpp \
           include/util/error.hpp \
           include/util/os.hpp \
           include/util/tr1.hpp \
           include/muxer/dec/ape_d.hpp \
           include/muxer/dec/wav_d.hpp \
           include/muxer/enc/mp3_e.hpp
SOURCES += src/khopper.cpp \
           src/info/audiofile.cpp \
           src/info/cuesheet.cpp \
           src/info/track.cpp \
           src/muxer/converter.cpp \
           src/muxer/decoder.cpp \
           src/muxer/encoder.cpp \
           src/ui/mainwindow.cpp \
           src/ui/songlist.cpp \
           src/ui/textcodec.cpp \
           src/ui/threads.cpp \
           src/muxer/dec/ape_d.cpp \
           src/muxer/dec/wav_d.cpp \
           src/muxer/enc/mp3_e.cpp

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
    LIBS += -lloki
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
