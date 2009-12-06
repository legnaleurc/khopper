# - Find Lame
# This module can be used to find Lame
# Once done this will define
#
# LAME_FOUND       If false, don't try to use Lame
#
# LAME_INCLUDE_DIR The Lame include directory
# LAME_LIBRARIES     Link these to use Lame

if(LAME_INCLUDE_DIR AND LAME_LIBRARIES)
    set(LAME_FOUND TRUE)
else()
    find_path(LAME_INCLUDE_DIR lame/lame.h)
    find_library(LAME_LIBRARIES mp3lame)

    if(LAME_INCLUDE_DIR AND LAME_LIBRARIES)
        set(LAME_FOUND TRUE)
    endif()

    if(LAME_FOUND)
        if(NOT Lame_FIND_QUIETLY)
            message(STATUS "Found Lame: ${LAME_LIBRARIES}")
        endif()
    else()
        if(Lame_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find Lame")
        else()
            if(NOT Lame_FIND_QUIETLY)
                message(STATUS "Could NOT find Lame")
            endif()
        endif()
    endif()
endif()
