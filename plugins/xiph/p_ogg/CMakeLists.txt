find_package(OggVorbis REQUIRED)

file(GLOB_RECURSE KHOPPER_PLUGIN_PANEL_OGG_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${KHOPPER_XIPH_DIR}/l_flac" ${OGGVORBIS_INCLUDE_DIR})
add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
add_definitions(-DKHOPPER_XIPH_LIB=${KHOPPER_XIPH_LIB})

qt4_wrap_cpp(KHOPPER_PLUGIN_PANEL_OGG_MOC_SOURCES oggpanel.hpp)

add_library(khopper_panel_ogg SHARED ${KHOPPER_PLUGIN_PANEL_OGG_SOURCES} ${KHOPPER_PLUGIN_PANEL_OGG_MOC_SOURCES})
khopper_add_plugin(khopper_panel_ogg)
target_link_libraries(khopper_panel_ogg ${OGGVORBIS_LIBRARIES})
