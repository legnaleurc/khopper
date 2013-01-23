# - Find OggVorbis
# This module can be used to find OggVorbis
# Once done this will define
#
# OGGVORBIS_FOUND         If false, don't try to use OggVorbis
#
# OGGVORBIS_INCLUDE_DIRS  Include directory necessary for using the OggVorbis headers
# OGGVORBIS_LIBRARIES     Link this to use OggVorbis

include(KhopperUtility)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_OGGVORBIS QUIET ogg vorbisenc)
    if(PC_OGGVORBIS_FOUND)
        set(OGGVORBIS_INCLUDE_DIRS ${PC_OGGVORBIS_INCLUDE_DIRS})
        set(OGGVORBIS_LIBRARIES ${PC_OGGVORBIS_LIBRARIES})
    endif()
endif()

path_from_env(OGGVORBIS_ROOT)

if(NOT OGGVORBIS_INCLUDE_DIRS)
    find_path(VORBIS_INCLUDE_DIR vorbis/vorbisenc.h
        HINTS ${OGGVORBIS_ROOT}
        PATH_SUFFIXES "include")
endif()

if(NOT OGGVORBIS_LIBRARIES)
    find_library(VORBIS_LIBRARY_RELEASE vorbis
        HINTS ${OGGVORBIS_ROOT}
        PATH_SUFFIXES "lib")
    find_library(VORBIS_LIBRARY_DEBUG vorbisd
        HINTS ${OGGVORBIS_ROOT}
        PATH_SUFFIXES "lib")
    find_library(OGG_LIBRARY_RELEASE ogg
        HINTS ${OGGVORBIS_ROOT}
        PATH_SUFFIXES "lib")
    find_library(OGG_LIBRARY_DEBUG oggd
        HINTS ${OGGVORBIS_ROOT}
        PATH_SUFFIXES "lib")

    include(SelectLibraryConfigurations)
    select_library_configurations(OGG)
    select_library_configurations(VORBIS)

    set(OGGVORBIS_INCLUDE_DIRS ${VORBIS_INCLUDE_DIR})
    set(OGGVORBIS_LIBRARIES ${OGG_LIBRARIES} ${VORBIS_LIBRARIES})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OGGVORBIS DEFAULT_MSG OGGVORBIS_INCLUDE_DIRS OGGVORBIS_LIBRARIES)

mark_as_advanced(OGGVORBIS_INCLUDE_DIRS OGGVORBIS_LIBRARIES)
