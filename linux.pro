######################################################################
# Automatically generated by qmake (2.01a) ?? ?? 28 12:25:33 2008
######################################################################

TEMPLATE = app
TARGET = Khopper
DEPENDPATH += . \
              src \
              src/conv \
              src/cue \
              src/ui \
              src/util \
              src/conv/input \
              src/conv/output \
              src/util/os
INCLUDEPATH += . \
               src/ui \
               src/cue \
               src/util \
               src/conv/output \
               src/conv \
               src/util/os \
               src/conv/input
LIBS += -lloki
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = build

# Input
HEADERS += src/conv/converter.hpp \
           src/cue/cue.hpp \
           src/ui/MainWindow.hpp \
           src/util/error.hpp \
           src/util/type.hpp \
           src/conv/output/mp3.hpp \
           src/conv/output/output.hpp \
           src/util/os/os.hpp
SOURCES += src/main.cpp \
           src/conv/converter.cpp \
           src/cue/cue.cpp \
           src/ui/MainWindow.cpp \
           src/conv/output/mp3.cpp \
           src/util/os/linux.cpp
