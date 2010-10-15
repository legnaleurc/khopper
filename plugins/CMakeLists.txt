if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/plugins")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib/plugins")
endif()

include_directories("${CMAKE_SOURCE_DIR}/core/include")

macro(KHOPPER_ADD_PLUGIN target)
    add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
    add_definitions(-DKHOPPER_PLUGIN_ID=${target})
    add_dependencies(${target} khopper_core)
    target_link_libraries(${target} khopper_core)
    set_target_properties(${target} PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
endmacro()

add_subdirectory(cuesheet)

find_package(FFmpeg)
if(FFMPEG_FOUND)
    message(STATUS "Found fallowing OPTIONAL package, FFmpeg plugin will be build.")
    add_subdirectory(ffmpeg)
endif()

find_package(Mp3Lame)
find_package(TagLib)
if(LAME_FOUND AND TAGLIB_FOUND)
    message(STATUS "Found fallowing OPTIONAL package, mp3Lame plugin will be build.")
    add_subdirectory(mp3lame)
endif()

add_subdirectory(redbook)

add_subdirectory(single)

find_package(Flac)
find_package(OggVorbis)
if(FLAC_FOUND AND OGGVORBIS_FOUND)
    message(STATUS "Found fallowing OPTIONAL package, Xiph plugin will be build.")
    add_subdirectory(xiph)
endif()
