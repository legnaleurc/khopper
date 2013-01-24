# - Find flac
# This module can be used to find flac
# Once done this will define
#
# FLAC_FOUND        If false, don't try to use flac
#
# FLAC_INCLUDE_DIRS The Flac include directory
# FLAC_LIBRARIES    Link these to use Flac

include(KhopperUtility)

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_FLAC QUIET flac)
    if(PC_FLAC_FOUND)
        set(FLAC_INCLUDE_DIRS ${PC_FLAC_INCLUDE_DIRS})
        set(FLAC_LIBRARIES ${PC_FLAC_LIBRARIES})
    endif()
endif()

path_from_env(FLAC_ROOT)

if(NOT FLAC_INCLUDE_DIRS)
    find_path(FLAC_INCLUDE_DIRS
        NAMES FLAC/stream_encoder.h FLAC/stream_decoder.h
        HINTS ${FLAC_ROOT}
        PATH_SUFFIXES "include")
endif()

if(NOT FLAC_LIBRARIES)
    find_library(FLAC_LIBRARY_RELEASE FLAC
        HINTS ${FLAC_ROOT}
        PATH_SUFFIXES "lib")
    find_library(FLAC_LIBRARY_DEBUG FLACd
        HINTS ${FLAC_ROOT}
        PATH_SUFFIXES "lib")

    include(SelectLibraryConfigurations)
    select_library_configurations(FLAC)
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FLAC DEFAULT_MSG FLAC_INCLUDE_DIRS FLAC_LIBRARIES)

mark_as_advanced(FLAC_INCLUDE_DIRS FLAC_LIBRARIES)
