set(KHOPPER_XIPH_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(KHOPPER_XIPH_LIB khopper_writer_flac)

add_subdirectory(l_flac)
add_subdirectory(p_flac)
add_subdirectory(p_ogg)
