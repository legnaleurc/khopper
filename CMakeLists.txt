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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -pedantic -Wextra -Wall -pipe")
elseif(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wextra -Wall -pipe")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /MP /nologo")
endif()

# debug version postfix
set(CMAKE_DEBUG_POSTFIX d)

if(MSVC)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_subdirectory(core)
add_subdirectory(gui)
add_subdirectory(plugins)

include(FeatureSummary)
feature_summary(WHAT ALL)
