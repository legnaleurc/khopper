# plugins

if(MSVC)
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/plugins")
else()
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib/plugins")
endif()

include_directories("${CMAKE_SOURCE_DIR}/core/include")

function(KHOPPER_ADD_PLUGIN target)
    add_definitions(${QT_DEFINITIONS} -DQT_PLUGIN -DQT_SHARED)
    add_definitions(-DKHOPPER_PLUGIN_ID=${target})
    add_dependencies(${target} khopper)
    target_link_libraries(${target} khopper)
    set_target_properties(${target} PROPERTIES VERSION ${KHOPPER_VERSION} SOVERSION ${KHOPPER_VERSION})
    install(TARGETS ${target}
        RUNTIME DESTINATION "plugins"
        LIBRARY DESTINATION "lib/khopper/plugins")
endfunction()

add_subdirectory(cuesheet)
add_subdirectory(ffmpeg)
add_subdirectory(libav)
add_subdirectory(mp3lame)
add_subdirectory(redbook)
add_subdirectory(single)
add_subdirectory(xiph)
