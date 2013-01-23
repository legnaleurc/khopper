# core

include(FeatureSummary)
include(KhopperUtility)

# Find Qt4
find_package(Qt4 4.7 REQUIRED)
set_package_properties(Qt4
    PROPERTIES
    TYPE REQUIRED
    PURPOSE "Necessary for core library")
include(${QT_USE_FILE})

#find TagLib
find_package(TagLib REQUIRED)
set_package_properties(TagLib
    PROPERTIES
    TYPE REQUIRED
    PURPOSE "Necessary for metadata reading")

# hack MSCV path
if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
endif()

add_definitions(-DKHOPPER_LIBRARY)

file(GLOB_RECURSE KHOPPER_CORE_PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "include/*.hpp")
file(GLOB_RECURSE KHOPPER_CORE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE KHOPPER_CORE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
group_sources("include")
group_sources("src")
if(CMAKE_HOST_WIN32)
    file(GLOB_RECURSE KHOPPER_CORE_PLATFORM_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} win32/*.cpp)
    group_sources("win32")
elseif(CMAKE_HOST_UNIX)
    file(GLOB_RECURSE KHOPPER_CORE_PLATFORM_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} unix/*.cpp)
    group_sources("unix")
endif()

find_moc_headers(KHOPPER_CORE_MOC_HEADERS ${KHOPPER_CORE_HEADERS})
qt4_wrap_cpp(KHOPPER_CORE_MOC_SOURCES ${KHOPPER_CORE_MOC_HEADERS})
source_group("Generated Files" FILES ${KHOPPER_CORE_MOC_SOURCES})

include_directories("include/khopper" "src" ${TAGLIB_INCLUDE_DIRS})

add_library(khopper SHARED ${KHOPPER_CORE_HEADERS} ${KHOPPER_CORE_SOURCES} ${KHOPPER_CORE_MOC_SOURCES} ${KHOPPER_CORE_PLATFORM_SOURCES})
target_link_libraries(khopper ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${TAGLIB_LIBRARIES})
set_target_properties(khopper
    PROPERTIES
        VERSION ${KHOPPER_VERSION}
        SOVERSION ${KHOPPER_VERSION}
        FRAMEWORK TRUE
        PUBLIC_HEADER "${KHOPPER_CORE_PUBLIC_HEADERS}")

install(TARGETS khopper
    RUNTIME DESTINATION "bin"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    FRAMEWORK DESTINATION "/Library/Frameworks"
    PUBLIC_HEADER DESTINATION "include/khopper")
