# - Find Loki
# This module can be used to find Loki
# Once done this will define
#
# LOKI_FOUND       If false, don't try to use Loki
#
# LOKI_INCLUDE_DIR The Loki include directory
# LOKI_LIBRARIES   Link these to use Loki

if(LOKI_INCLUDE_DIR AND LOKI_LIBRARIES)
    set(LOKI_FOUND TRUE)
else()
    find_path(LOKI_INCLUDE_DIR
        NAMES loki/Factory.h loki/Singleton.h)
    find_library(LOKI_LIBRARIES_RELEASE loki)
    find_library(LOKI_LIBRARIES_DEBUG lokid)
    if(LOKI_LIBRARIES_RELEASE AND LOKI_LIBRARIES_DEBUG)
        set(LOKI_LIBRARIES optimized ${LOKI_LIBRARIES_RELEASE} debug ${LOKI_LIBRARIES_DEBUG})
    elseif(LOKI_LIBRARIES_RELEASE)
        set(LOKI_LIBRARIES ${LOKI_LIBRARIES_RELEASE})
    endif()

    if(LOKI_INCLUDE_DIR AND LOKI_LIBRARIES)
        set(LOKI_FOUND TRUE)
    endif()

    if(LOKI_FOUND)
        set(LOKI_LIBRARIES ${LOKI_LIBRARIES} CACHE STRING "Loki libraries" FORCE)
        if(NOT Loki_FIND_QUIETLY)
            message(STATUS "Found Loki: ${LOKI_LIBRARIES}")
        endif()
    else()
        if(Loki_FIND_REQUIRED)
            message(FATAL_ERROR "Could NOT find Loki")
        else()
            if(NOT Loki_FIND_QUIETLY)
                message(STATUS "Could NOT find Loki")
            endif()
        endif()
    endif()
endif()
