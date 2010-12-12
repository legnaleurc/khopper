# main

# hack MSVC path
if(MSVC)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
endif()

file(GLOB_RECURSE KHOPPER_GUI_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE KHOPPER_GUI_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE KHOPPER_GUI_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.ui)
include_directories(
    src/utility
    src/widget
    ${CMAKE_CURRENT_BINARY_DIR}
    "${CMAKE_SOURCE_DIR}/core/include"
    ${QT_PHONON_INCLUDE_DIR})
set(KHOPPER_GUI_MOC_HEADERS
    src/utility/converter.hpp
    src/widget/conversiondialog.hpp
    src/widget/mainwindow.hpp
    src/widget/player.hpp
    src/widget/playlistview.hpp
    src/widget/preference.hpp
    src/widget/progressbar.hpp
    src/widget/progressviewer.hpp
    src/widget/propertydialog.hpp)

qt4_wrap_cpp(KHOPPER_GUI_MOC_SOURCES ${KHOPPER_GUI_MOC_HEADERS})
qt4_wrap_ui(KHOPPER_GUI_UIC_HEADERS ${KHOPPER_GUI_FORMS})
qt4_add_resources(KHOPPER_GUI_RCC_SOURCES main.qrc)

source_group("Generated Files" FILES ${KHOPPER_GUI_MOC_SOURCES} ${KHOPPER_GUI_RCC_SOURCES} ${KHOPPER_GUI_UIC_HEADERS})
source_group("Resource Files" REGULAR_EXPRESSION .*\\.rc)
source_group("Form Files" REGULAR_EXPRESSION .*\\.ui)

if(MSVC)
    add_executable(khopper_gui WIN32 main.rc ${KHOPPER_GUI_HEADERS} ${KHOPPER_GUI_SOURCES} ${KHOPPER_GUI_MOC_SOURCES} ${KHOPPER_GUI_UIC_HEADERS} ${KHOPPER_GUI_RCC_SOURCES})
    target_link_libraries(khopper_gui khopper ${QT_LIBRARIES} ${QT_PHONON_LIBRARY} ${QT_QTMAIN_LIBRARY})
else()
    add_executable(khopper_gui ${KHOPPER_GUI_HEADERS} ${KHOPPER_GUI_SOURCES} ${KHOPPER_GUI_MOC_SOURCES} ${KHOPPER_GUI_UIC_HEADERS} ${KHOPPER_GUI_RCC_SOURCES})
    target_link_libraries(khopper_gui khopper ${QT_LIBRARIES} ${QT_PHONON_LIBRARY})
endif()
add_dependencies(khopper_gui khopper)
set_target_properties(khopper_gui PROPERTIES VERSION ${KHOPPER_VERSION})

install(TARGETS khopper_gui DESTINATION "bin")
if(UNIX)
    install(FILES share/applications/khopper.desktop DESTINATION "share/applications")
    install(FILES share/pixmap/logo.png DESTINATION "share/pixmap" RENAME "khopper.png")
    install(SCRIPT postinstall.cmake)
endif()
