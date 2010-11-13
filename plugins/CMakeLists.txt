# plugins

if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/plugins")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib/plugins")
endif()

include_directories("${CMAKE_SOURCE_DIR}/core/include")

macro(KHOPPER_ADD_PLUGIN target)
    add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
    add_definitions(-DKHOPPER_PLUGIN_ID=${target})
    add_dependencies(${target} libkhopper)
    target_link_libraries(${target} libkhopper)
    set_target_properties(${target} PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
    install(TARGETS ${target}
        RUNTIME DESTINATION "plugins"
        LIBRARY DESTINATION "lib/khopper/plugins")
endmacro()

add_subdirectory(cuesheet)

find_package(FFmpeg)
if(FFMPEG_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "Found libav*, FFmpeg plugin will be build.")
    add_subdirectory(ffmpeg)
else()
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "libav* for FFmpeg plugin")
endif()

find_package(Mp3Lame)
find_package(TagLib)
if(MP3LAME_FOUND AND TAGLIB_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "Found libmp3lame and TagLib, mp3Lame plugin will be build.")
    add_subdirectory(mp3lame)
else()
    if(NOT MP3LAME_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "libmp3lame for mp3Lame plugin")
    endif()
    if(NOT TAGLIB_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "TagLib for mp3Lame plugin")
    endif()
endif()

add_subdirectory(redbook)

add_subdirectory(single)

find_package(Flac)
find_package(OggVorbis)
if(FLAC_FOUND AND OGGVORBIS_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "Found libFLAC and OggVorbis, Xiph plugin will be build.")
    add_subdirectory(xiph)
else()
    if(NOT FLAC_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "libFLAC for Xiph plugin")
    endif()
    if(NOT OGGVORBIS_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "OggVorbis for Xiph plugin")
    endif()
endif()

if(QT_QTWEBKIT_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "Found QtWebKit module, YouTube plugin will be build.")
    add_subdirectory(youtube)
else()
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "QtWebKit not found for YouTube plugin.")
endif()

set(KHOPPER_OPTIONAL_FOUND ${KHOPPER_OPTIONAL_FOUND} PARENT_SCOPE)
set(KHOPPER_OPTIONAL_NOT_FOUND ${KHOPPER_OPTIONAL_NOT_FOUND} PARENT_SCOPE)
