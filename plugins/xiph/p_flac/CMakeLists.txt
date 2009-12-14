file(GLOB_RECURSE KHOPPER_PLUGIN_PANEL_FLAC_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

include_directories("${KHOPPER_XIPH_DIR}/l_flac")
add_definitions(-DKHOPPER_XIPH_LIB=${KHOPPER_XIPH_LIB})

qt4_wrap_cpp(KHOPPER_PLUGIN_PANEL_FLAC_MOC_SOURCES flacpanel.hpp)

add_library(khopper_panel_flac SHARED ${KHOPPER_PLUGIN_PANEL_FLAC_SOURCES} ${KHOPPER_PLUGIN_PANEL_FLAC_MOC_SOURCES})
khopper_add_plugin(khopper_panel_flac)
