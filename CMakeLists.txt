# root

cmake_minimum_required(VERSION 2.8)

project(Khopper)
set(KHOPPER_VERSION_MAJOR "0")
set(KHOPPER_VERSION_MINOR "5")
set(KHOPPER_VERSION_PATCH "0")
set(KHOPPER_VERSION "${KHOPPER_VERSION_MAJOR}.${KHOPPER_VERSION_MINOR}.${KHOPPER_VERSION_PATCH}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

add_definitions(-DKHOPPER_VERSION=${KHOPPER_VERSION} -DKHOPPER_VERSION_MAJOR=${KHOPPER_VERSION_MAJOR} -DKHOPPER_VERSION_MINOR=${KHOPPER_VERSION_MINOR} -DKHOPPER_VERSION_PATCH=${KHOPPER_VERSION_PATCH})
foreach(type "RELEASE" "MINSIZEREL")
    set(CMAKE_CXX_FLAGS_${type} "${CMAKE_CXX_FLAGS_${type}} -DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
endforeach()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++ -pedantic -Wextra -Wall -pipe")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -pedantic -Wextra -Wall -pipe")
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

# debug version postfix
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(plugins)

include(FeatureSummary)
feature_summary(WHAT ALL)
