unix {
    INC_DIRS = $$system( find ./include -maxdepth 1 -type d )
    SRC_DIRS = $$system( find ./src -maxdepth 1 -type d )

    HEADERS += $$system( find ./include -name '*.hpp' )
    SOURCES += $$system( find ./src -name '*.cpp' )
    SOURCES -= ./src/common/windows.cpp
}

DEPENDPATH  += $$INC_DIRS $$SRC_DIRS
INCLUDEPATH += $$INC_DIRS

# Input
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
