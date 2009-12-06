# - Find OggVorbis
# This module can be used to find OggVorbis
# Once done this will define
#
# OGGVORBIS_FOUND         If false, don't try to use OggVorbis
#
# OGGVORBIS_INCLUDE_DIR   Include directory necessary for using the OggVorbis headers
# OGGVORBIS_LIBRARIES     Link this to use OggVorbis

if(OGGVORBIS_INCLUDE_DIR AND OGGVORBIS_LIBRARIES)
    set(OGGVORBIS_FOUND TRUE)
else()
    if(NOT WIN32)
        find_package(PkgConfig)
        pkg_check_modules(PC_OGGVORBIS ogg vorbis)
    endif()

    find_path(VORBIS_INCLUDE_DIR vorbis/vorbisenc.h
        HINTS
            ${PC_OGGVORBIS_vorbis_INCLUDEDIR}
            ${PC_OGGVORBIS_vorbis_INCLUDE_DIRS})
    find_library(VORBIS_LIBRARY vorbis
        HINTS
            ${PC_OGGVORBIS_vorbis_LIBDIR}
            ${PC_OGGVORBIS_vorbis_LIBRARY_DIRS})

    set(OGGVORBIS_INCLUDE_DIR)
    if(VORBIS_INCLUDE_DIR)
        set(OGGVORBIS_INCLUDE_DIR ${OGGVORBIS_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})
    endif()

    set(OGGVORBIS_LIBRARIES)
    if(VORBIS_LIBRARY)
        set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} ${VORBIS_LIBRARY})
    endif()

    if(OGGVORBIS_INCLUDE_DIR AND OGGVORBIS_LIBRARIES)
        set(OGGVORBIS_FOUND TRUE)
    endif()

    if(OGGVORBIS_FOUND)
        if(NOT OggVorbis_FIND_QUIETLY)
            message(STATUS "Found OggVorbis: ${OGGVORBIS_LIBRARIES} ${OGGVORBIS_INCLUDE_DIR}")
        endif()
    else()
        if(OggVorbis_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find OggVorbis")
        else()
            if(NOT OggVorbis_FIND_QUIETLY)
                message(STATUS "Could NOT find OggVorbis")
            endif()
        endif()
    endif()

    mark_as_advanced(VORBIS_INCLUDE_DIR)
endif()
