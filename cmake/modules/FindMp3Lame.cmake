# - Find Lame
# This module can be used to find Lame
# Once done this will define
#
# MP3LAME_FOUND       If false, don't try to use Lame
#
# MP3LAME_INCLUDE_DIR The Lame include directory
# MP3LAME_LIBRARIES     Link these to use Lame

if(MP3LAME_INCLUDE_DIR AND MP3LAME_LIBRARIES)
    set(MP3LAME_FOUND TRUE)
else()
    find_path(MP3LAME_INCLUDE_DIR lame/lame.h)
    find_library(MP3LAME_LIBRARIES_RELEASE mp3lame)
    find_library(MP3LAME_LIBRARIES_DEBUG mp3lamed)
    if(MP3LAME_LIBRARIES_RELEASE AND MP3LAME_LIBRARIES_DEBUG)
        set(MP3LAME_LIBRARIES optimized ${MP3LAME_LIBRARIES_RELEASE} debug ${MP3LAME_LIBRARIES_DEBUG})
    elseif(MP3LAME_LIBRARIES_RELEASE)
        set(MP3LAME_LIBRARIES ${MP3LAME_LIBRARIES_RELEASE})
    endif()

    if(MP3LAME_INCLUDE_DIR AND MP3LAME_LIBRARIES)
        set(MP3LAME_FOUND TRUE)
    endif()

    if(MP3LAME_FOUND)
        set(MP3LAME_LIBRARIES ${MP3LAME_LIBRARIES} CACHE STRING "Lame libraries" FORCE)
        if(NOT Mp3Lame_FIND_QUIETLY)
            message(STATUS "Found Lame: ${MP3LAME_LIBRARIES}")
        endif()
    else()
        if(Mp3Lame_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find Lame")
        else()
            if(NOT Mp3Lame_FIND_QUIETLY)
                message(STATUS "Could NOT find Lame")
            endif()
        endif()
    endif()
endif()
