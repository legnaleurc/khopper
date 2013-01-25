# xiph plugin

find_package(Flac)
set_package_properties(Flac
    PROPERTIES
    TYPE OPTIONAL
    PURPOSE "Enables FLAC decoder/encoder")
find_package(OggVorbis)
set_package_properties(OggVorbis
    PROPERTIES
    TYPE OPTIONAL
    PURPOSE "Enables ogg vorbis decoder/encoder")

if(FLAC_FOUND AND OGGVORBIS_FOUND)
    include(KhopperUtility)

    file(GLOB_RECURSE KHOPPER_PLUGIN_XIPH_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hpp")
    file(GLOB_RECURSE KHOPPER_PLUGIN_XIPH_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cpp")
    file(GLOB_RECURSE KHOPPER_PLUGIN_XIPH_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "src/*.ui")
    group_sources("src")

    include_directories(${FLAC_INCLUDE_DIR} ${OGGVORBIS_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

    qt4_wrap_ui(KHOPPER_PLUGIN_XIPH_UIC_HEADERS ${KHOPPER_PLUGIN_XIPH_FORMS})

    source_group("Generated Files" FILES ${KHOPPER_PLUGIN_XIPH_UIC_HEADERS})

    add_library(khopper_plugin_xiph SHARED ${KHOPPER_PLUGIN_XIPH_HEADERS} ${KHOPPER_PLUGIN_XIPH_SOURCES} ${KHOPPER_PLUGIN_XIPH_UIC_HEADERS})
    target_link_libraries(khopper_plugin_xiph ${FLAC_LIBRARIES} ${OGGVORBIS_LIBRARIES})
    khopper_add_plugin(khopper_plugin_xiph)
endif()
