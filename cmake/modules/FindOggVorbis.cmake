# - Find OggVorbis
# This module can be used to find OggVorbis
# Once done this will define
#
# OGGVORBIS_FOUND         If false, don't try to use OggVorbis
#
# OGGVORBIS_INCLUDE_DIRS   Include directory necessary for using the OggVorbis headers
# OGGVORBIS_LIBRARIES     Link this to use OggVorbis

if(NOT OGGVORBIS_FOUND)
    find_package(PkgConfig)
    if(PKG_CONFIG_FOUND)
        pkg_check_modules(PC_OGGVORBIS ogg vorbisenc)
        if(PC_OGGVORBIS_FOUND)
            set(OGGVORBIS_INCLUDE_DIRS ${PC_OGGVORBIS_INCLUDE_DIRS} CACHE INTERNAL "")
            set(OGGVORBIS_LIBRARIES ${PC_OGGVORBIS_LIBRARIES} CACHE INTERNAL "")
            set(OGGVORBIS_FOUND TRUE CACHE INTERNAL "")
        endif()
    else()
        find_path(VORBIS_INCLUDE_DIR vorbis/vorbisenc.h)
        find_library(VORBIS_LIBRARY_RELEASE vorbis)
        find_library(VORBIS_LIBRARY_DEBUG vorbisd)
        find_library(OGG_LIBRARY_RELEASE ogg)
        find_library(OGG_LIBRARY_DEBUG oggd)
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

        set(OGGVORBIS_INCLUDE_DIRS)
        if(VORBIS_INCLUDE_DIR)
            set(OGGVORBIS_INCLUDE_DIRS ${OGGVORBIS_INCLUDE_DIRS} ${VORBIS_INCLUDE_DIR})
        endif()

        unset(OGGVORBIS_LIBRARIES CACHE)
        if(OGG_LIBRARY)
            set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} ${OGG_LIBRARY})
        endif()
        if(VORBIS_LIBRARY)
            set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} ${VORBIS_LIBRARY})
        endif()

        if(OGGVORBIS_INCLUDE_DIRS AND OGG_LIBRARY AND VORBIS_LIBRARY)
            set(OGGVORBIS_FOUND TRUE)
        endif()
    endif()

    if(OGGVORBIS_FOUND)
        set(OGGVORBIS_INCLUDE_DIRS ${OGGVORBIS_INCLUDE_DIRS} CACHE STRING "OggVorbis include directories" FORCE)
        set(OGGVORBIS_LIBRARIES ${OGGVORBIS_LIBRARIES} CACHE STRING "OggVorbis libraries" FORCE)
        if(NOT OggVorbis_FIND_QUIETLY)
            message(STATUS "Found OggVorbis: ${OGGVORBIS_LIBRARIES} ${OGGVORBIS_INCLUDE_DIRS}")
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
