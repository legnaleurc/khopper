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
    add_dependencies(${target} khopper)
    target_link_libraries(${target} khopper)
    set_target_properties(${target} PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
    install(TARGETS ${target}
        RUNTIME DESTINATION "plugins"
        LIBRARY DESTINATION "lib/khopper/plugins")
endmacro()

add_subdirectory(cuesheet)

find_package(FFmpeg)
if(FFMPEG_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "[Plugin] FFmpeg plugin enabled.")
    add_subdirectory(ffmpeg)
else()
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "[Plugin] FFmpeg plugin disabled:")
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "    libav* not found.")
endif()

find_package(Mp3Lame)
if(MP3LAME_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "[Plugin] mp3Lame plugin enabled.")
    add_subdirectory(mp3lame)
else()
    list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "[Plugin] mp3Lame plugin disabled:")
    if(NOT MP3LAME_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "    libmp3lame not found.")
    endif()
endif()

list(APPEND KHOPPER_OPTIONAL_FOUND "[Plugin] Readbook plugin enabled.")
add_subdirectory(redbook)

list(APPEND KHOPPER_OPTIONAL_FOUND "[Plugin] Single plugin enabled.")
add_subdirectory(single)

find_package(Flac)
find_package(OggVorbis)
if(FLAC_FOUND AND OGGVORBIS_FOUND)
    list(APPEND KHOPPER_OPTIONAL_FOUND "[Plugin] Xiph plugin enabled.")
    add_subdirectory(xiph)
else()
    list(APPEND KHOPPER_OPTIONAL_FOUND "[Plugin] Xiph plugin disabled:")
    if(NOT FLAC_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "    libFLAC not found.")
    endif()
    if(NOT OGGVORBIS_FOUND)
        list(APPEND KHOPPER_OPTIONAL_NOT_FOUND "    OggVorbis not found.")
    endif()
endif()

set(KHOPPER_OPTIONAL_FOUND ${KHOPPER_OPTIONAL_FOUND} PARENT_SCOPE)
set(KHOPPER_OPTIONAL_NOT_FOUND ${KHOPPER_OPTIONAL_NOT_FOUND} PARENT_SCOPE)
