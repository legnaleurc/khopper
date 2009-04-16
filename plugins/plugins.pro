TEMPLATE  = subdirs
SUBDIRS  += l_default p_flac p_mp3 p_ogg r_default w_default
CONFIG   += ordered debug_and_release

win32:SUBDIRS += l_default
