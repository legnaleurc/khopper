# plugins

include_directories("${CMAKE_SOURCE_DIR}/core/include")

function(KHOPPER_ADD_PLUGIN target)
    find_package(Qt4 4.7 QUIET)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
    add_definitions(-DKHOPPER_PLUGIN_ID=${target})
    add_dependencies(${target} khopper)
    target_link_libraries(${target} khopper ${QT_LIBRARIES})
    set_target_properties(${target} PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
    install(TARGETS ${target}
        RUNTIME DESTINATION "plugins"
        LIBRARY DESTINATION "lib/khopper/plugins")
endfunction()

option(KHOPPER_USE_LIBAV "Use Libav instead of FFmpeg. (imply disable ffmpeg)" FALSE)

add_subdirectory(cuesheet)
if(NOT KHOPPER_USE_LIBAV)
    add_subdirectory(ffmpeg)
else()
    add_subdirectory(libav)
endif()
add_subdirectory(mp3lame)
add_subdirectory(redbook)
add_subdirectory(single)
add_subdirectory(xiph)
