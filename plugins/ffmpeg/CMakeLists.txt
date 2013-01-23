# ffmpeg plugin

find_package(FFmpeg)
set_package_properties(FFmpeg
    PROPERTIES
    TYPE RECOMMENDED
    PURPOSE "Enables generic codecs")

if(FFMPEG_FOUND)
    include(KhopperUtility)

    file(GLOB_RECURSE KHOPPER_PLUGIN_FFMPEG_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.hpp)
    file(GLOB_RECURSE KHOPPER_PLUGIN_FFMPEG_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
    file(GLOB_RECURSE KHOPPER_PLUGIN_FFMPEG_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.ui)
    group_sources("src")
    if(WIN32)
        list(APPEND KHOPPER_PLUGIN_FFMPEG_HEADERS win/wfile.hpp win/inttypes.h)
        list(APPEND KHOPPER_PLUGIN_FFMPEG_SOURCES win/wfile.cpp)
        include_directories("win")
        group_sources("win")
    endif()

    include_directories(${CMAKE_CURRENT_BINARY_DIR} ${FFMPEG_INCLUDE_DIRS})

    qt4_wrap_ui(KHOPPER_PLUGIN_FFMPEG_UIC_HEADERS ${KHOPPER_PLUGIN_FFMPEG_FORMS})

    source_group("Generated Files" FILES ${KHOPPER_PLUGIN_FFMPEG_UIC_HEADERS})

    add_library(khopper_plugin_ffmpeg SHARED ${KHOPPER_PLUGIN_FFMPEG_HEADERS} ${KHOPPER_PLUGIN_FFMPEG_SOURCES} ${KHOPPER_PLUGIN_FFMPEG_UIC_HEADERS})
    target_link_libraries(khopper_plugin_ffmpeg ${FFMPEG_LIBRARIES})
    khopper_add_plugin(khopper_plugin_ffmpeg)
endif()
