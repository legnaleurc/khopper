# mp3lame plugin

find_package(Lame REQUIRED)
find_package(TagLib REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_MP3LAME_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.hpp)
file(GLOB_RECURSE KHOPPER_PLUGIN_MP3LAME_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE KHOPPER_PLUGIN_MP3LAME_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.ui)

include_directories(
    ${LAME_INCLUDE_DIR}
    ${TAGLIB_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

qt4_wrap_ui(KHOPPER_PLUGIN_MP3LAME_UIC_HEADERS ${KHOPPER_PLUGIN_MP3LAME_FORMS})

source_group("Generated Files" FILES ${KHOPPER_PLUGIN_MP3LAME_UIC_HEADERS})
source_group("Form Files" REGULAR_EXPRESSION .*\\.ui)

add_library(khopper_plugin_mp3lame SHARED ${KHOPPER_PLUGIN_MP3LAME_HEADERS} ${KHOPPER_PLUGIN_MP3LAME_SOURCES} ${KHOPPER_PLUGIN_MP3LAME_UIC_HEADERS})
khopper_add_plugin(khopper_plugin_mp3lame)
target_link_libraries(khopper_plugin_mp3lame ${LAME_LIBRARIES} ${TAGLIB_LIBRARIES})

install(TARGETS khopper_plugin_mp3lame DESTINATION
    RUNTIME DESTINATION "plugins"
    LIBRARY DESTINATION "plugins"
    ARCHIVE DESTINATION "lib")