######################################################################
# Automatically generated by qmake (2.01a) ? 8 21 12:41:45 2008
######################################################################

TEMPLATE = vcapp
TARGET = khopper
DEPENDPATH += . \
              include \
              src \
              include\conv \
              include\cue \
              include\ui \
              include\util \
              src\conv \
              src\cue \
              src\ui \
              src\util \
              include\conv\input \
              include\conv\output \
              src\conv\input \
              src\conv\output
INCLUDEPATH += . \
               include

# Input
HEADERS += include/types.hpp \
           include/conv/converter.hpp \
           include/cue/audiofile.hpp \
           include/cue/cuesheet.hpp \
           include/cue/track.hpp \
           include/ui/mainwindow.hpp \
           include/ui/songlist.hpp \
           include/ui/threads.hpp \
           include/util/error.hpp \
           include/util/os.hpp \
           include/conv/input/input.hpp \
           include/conv/input/input_ape.hpp \
           include/conv/input/input_wav.hpp \
           include/conv/output/output.hpp \
           include/conv/output/output_mp3.hpp
SOURCES += src/main.cpp \
           src/conv/converter.cpp \
           src/cue/audiofile.cpp \
           src/cue/cuesheet.cpp \
           src/cue/track.cpp \
           src/ui/mainwindow.cpp \
           src/ui/songlist.cpp \
           src/ui/threads.cpp \
           src/util/windows.cpp \
           src/conv/input/input.cpp \
           src/conv/input/input_ape.cpp \
           src/conv/input/input_wav.cpp \
           src/conv/output/output.cpp \
           src/conv/output/output_mp3.cpp


LIBS += -lboost_regex-vc90-mt-1_35 \
        -lloki
OBJECTS_DIR = obj
MOC_DIR = moc
DESTDIR = build
DEFINES += LOKI_OBJECT_LEVEL_THREADING
