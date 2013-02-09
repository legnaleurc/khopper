# mp3lame plugin

find_package(TagLib)
find_package(Mp3Lame)
set_package_properties(Mp3Lame
    PROPERTIES
    TYPE OPTIONAL
    PURPOSE "Enables mp3 encoder")

if(NOT TAGLIB_FOUND OR NOT MP3LAME_FOUND)
    return()
endif()

include(KhopperUtility)

file(GLOB_RECURSE KHOPPER_PLUGIN_MP3LAME_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
file(GLOB_RECURSE KHOPPER_PLUGIN_MP3LAME_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE KHOPPER_PLUGIN_MP3LAME_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.ui")
group_sources("src")

include_directories(
${MP3LAME_INCLUDE_DIRS}
${TAGLIB_INCLUDE_DIRS}
${CMAKE_CURRENT_BINARY_DIR})

qt4_wrap_ui(KHOPPER_PLUGIN_MP3LAME_UIC_HEADERS ${KHOPPER_PLUGIN_MP3LAME_FORMS})

source_group("Generated Files" FILES ${KHOPPER_PLUGIN_MP3LAME_UIC_HEADERS})

add_library(khopper_plugin_mp3lame SHARED ${KHOPPER_PLUGIN_MP3LAME_HEADERS} ${KHOPPER_PLUGIN_MP3LAME_SOURCES} ${KHOPPER_PLUGIN_MP3LAME_UIC_HEADERS})
target_link_libraries(khopper_plugin_mp3lame ${MP3LAME_LIBRARIES} ${TAGLIB_LIBRARIES})
khopper_add_plugin(khopper_plugin_mp3lame)
