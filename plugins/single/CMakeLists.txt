# single plugin

include(KhopperUtility)

file(GLOB_RECURSE KHOPPER_PLUGIN_SINGLE_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
file(GLOB_RECURSE KHOPPER_PLUGIN_SINGLE_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
group_sources("src")

add_library(khopper_plugin_single SHARED ${KHOPPER_PLUGIN_SINGLE_HEADERS} ${KHOPPER_PLUGIN_SINGLE_SOURCES})
khopper_add_plugin(khopper_plugin_single)
