# root

cmake_minimum_required(VERSION 2.6)

project(Khopper)
set(KHOPPER_VERSION_MAJOR "0")
set(KHOPPER_VERSION_MINOR "2")
set(KHOPPER_VERSION_PATCH "60")
set(KHOPPER_VERSION "${KHOPPER_VERSION_MAJOR}.${KHOPPER_VERSION_MINOR}.${KHOPPER_VERSION_PATCH}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(Qt4 4.6 REQUIRED)
find_package(Boost REQUIRED)

include(${QT_USE_FILE})
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-DKHOPPER_VERSION=${KHOPPER_VERSION})
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    add_definitions(-DQT_NO_DEBUG -DQT_NO_DEBUG_OUTPUT)
endif()
if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++0x)
elseif(MSVC)
    add_definitions(/Zc:wchar_t- "/wd4127;4819")
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    if(CMAKE_CXX_FLAGS_RELEASE MATCHES "/O.")
        string(REGEX REPLACE "/O." "/Ox" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    else()
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    endif()
endif()
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(core)
add_subdirectory(main)
add_subdirectory(plugins)
