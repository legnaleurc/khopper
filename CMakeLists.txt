# root

cmake_minimum_required(VERSION 2.6)

project(Khopper)
set(KHOPPER_VERSION_MAJOR "0")
set(KHOPPER_VERSION_MINOR "4")
set(KHOPPER_VERSION_PATCH "2")
set(KHOPPER_VERSION "${KHOPPER_VERSION_MAJOR}.${KHOPPER_VERSION_MINOR}.${KHOPPER_VERSION_PATCH}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Qt4 4.7 COMPONENTS Network Phonon REQUIRED)

include(${QT_USE_FILE})

add_definitions(-DKHOPPER_VERSION=${KHOPPER_VERSION})
foreach(type "RELEASE" "MINSIZEREL")
    set(CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}} -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
endforeach()
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wall -pipe")
elseif(MSVC)
    # general flags
    add_definitions("/D__STDC_CONSTANT_MACROS")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /wd4127 /wd4819 /MP /nologo")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    # release flags
    foreach(type "RELEASE" "MINSIZEREL")
        if(CMAKE_CXX_FLAGS_${type} MATCHES "/O[0-3d]")
            string(REGEX REPLACE "/O[0-3d]" "/Ox" CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}}")
        else()
            set(CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}} /Ox")
        endif()
    endforeach()
endif()
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(core)

if(QT_PHONON_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "[GUI] GUI module enabled.")
    add_subdirectory(gui)
else()
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "[GUI] GUI module disabled:")
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "    Qt Phonon not found.")
endif()

add_subdirectory(plugins)

list(LENGTH KHOPPER_OPTIONAL_FOUND length)
if(NOT ${length} EQUAL 0)
    message("[Find the fallowing build dependencies:]")
    foreach(msg ${KHOPPER_OPTIONAL_FOUND})
        message(${msg})
    endforeach()
endif()
list(LENGTH KHOPPER_OPTIONAL_NOT_FOUND length)
if(NOT ${length} EQUAL 0)
    message("[The fallowing build dependencies are not found:]")
    foreach(msg ${KHOPPER_OPTIONAL_NOT_FOUND})
        message(${msg})
    endforeach()
endif()
