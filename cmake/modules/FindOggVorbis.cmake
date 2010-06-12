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
    find_library(VORBIS_LIBRARY_RELEASE vorbis
        HINTS
            ${PC_OGGVORBIS_vorbis_LIBDIR}
            ${PC_OGGVORBIS_vorbis_LIBRARY_DIRS})
    find_library(VORBIS_LIBRARY_DEBUG vorbisd
        HINTS
            ${PC_OGGVORBIS_vorbis_LIBDIR}
            ${PC_OGGVORBIS_vorbis_LIBRARY_DIRS})
    find_library(OGG_LIBRARY_RELEASE ogg
        HINTS
            ${PC_OGGVORBIS_ogg_LIBDIR}
            ${PC_OGGVORBIS_ogg_LIBRARY_DIRS})
    find_library(OGG_LIBRARY_DEBUG oggd
        HINTS
            ${PC_OGGVORBIS_ogg_LIBDIR}
            ${PC_OGGVORBIS_ogg_LIBRARY_DIRS})
    if(VORBIS_LIBRARY_RELEASE AND VORBIS_LIBRARY_DEBUG)
        set(VORBIS_LIBRARY optimized ${VORBIS_LIBRARY_RELEASE} debug ${VORBIS_LIBRARY_DEBUG})
    elseif(VORBIS_LIBRARY_RELEASE)
        set(VORBIS_LIBRARY ${VORBIS_LIBRARY_RELEASE})
    endif()
    if(OGG_LIBRARY_RELEASE AND OGG_LIBRARY_DEBUG)
        set(OGG_LIBRARY optimized ${OGG_LIBRARY_RELEASE} debug ${OGG_LIBRARY_DEBUG})
    elseif(OGG_LIBRARY_RELEASE)
        set(OGG_LIBRARY ${OGG_LIBRARY_RELEASE})
    endif()

    set(OGGVORBIS_INCLUDE_DIR)
    if(VORBIS_INCLUDE_DIR)
        set(OGGVORBIS_INCLUDE_DIR ${OGGVORBIS_INCLUDE_DIR} ${VORBIS_INCLUDE_DIR})
    endif()

    unset(OGGVORBIS_LIBRARIES CACHE)
    if(OGG_LIBRARY)
        set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} ${OGG_LIBRARY})
    endif()
    if(VORBIS_LIBRARY)
        set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} ${VORBIS_LIBRARY})
    endif()

    if(OGGVORBIS_INCLUDE_DIR AND OGG_LIBRARY AND VORBIS_LIBRARY)
        set(OGGVORBIS_FOUND TRUE)
    endif()

    if(OGGVORBIS_FOUND)
        set(OGGVORBIS_INCLUDE_DIR ${OGGVORBIS_INCLUDE_DIR} CACHE STRING "OggVorbis include directories" FORCE)
        set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} CACHE STRING "OggVorbis libraries" FORCE)
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
endif()
