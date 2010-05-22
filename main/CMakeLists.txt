# main

# use phonon
set(QT_USE_PHONON TRUE)
# hack MSVC path
if(MSVC)
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
else()
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
endif()

file(GLOB_RECURSE KHOPPER_MAIN_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE KHOPPER_MAIN_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE KHOPPER_MAIN_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.ui)
include_directories(
    src/album
    src/widget
    ${CMAKE_CURRENT_BINARY_DIR}
    "${CMAKE_SOURCE_DIR}/core/include"
    ${QT_PHONON_INCLUDE_DIR})
set(KHOPPER_MAIN_MOC_HEADERS
    src/widget/codecselector.hpp
    src/widget/conversiondialog.hpp
    src/widget/converter.hpp
    src/widget/converterthread.hpp
    src/widget/mainwindow.hpp
    src/widget/player.hpp
    src/widget/preference.hpp
    src/widget/progress.hpp
    src/widget/propertieswidget.hpp
    src/widget/seekslider.hpp
    src/widget/songlist.hpp)

qt4_wrap_cpp(KHOPPER_MAIN_MOC_SOURCES ${KHOPPER_MAIN_MOC_HEADERS})
qt4_wrap_ui(KHOPPER_MAIN_UIC_HEADERS ${KHOPPER_MAIN_FORMS})
qt4_add_resources(KHOPPER_MAIN_RCC_SOURCES main.qrc)

source_group("Generated Files" FILES ${KHOPPER_MAIN_MOC_SOURCES} ${KHOPPER_MAIN_RCC_SOURCES} ${KHOPPER_MAIN_UIC_HEADERS})
source_group("Resource Files" REGULAR_EXPRESSION .*\\.rc)
source_group("Form Files" REGULAR_EXPRESSION .*\\.ui)

if(MSVC)
    add_executable(khopper WIN32 main.rc ${KHOPPER_MAIN_HEADERS} ${KHOPPER_MAIN_SOURCES} ${KHOPPER_MAIN_MOC_SOURCES} ${KHOPPER_MAIN_UIC_HEADERS} ${KHOPPER_MAIN_RCC_SOURCES})
    target_link_libraries(khopper khopper_core ${QT_LIBRARIES} ${QT_PHONON_LIBRARY} ${QT_QTMAIN_LIBRARY})
else()
    add_executable(khopper ${KHOPPER_MAIN_HEADERS} ${KHOPPER_MAIN_SOURCES} ${KHOPPER_MAIN_MOC_SOURCES} ${KHOPPER_MAIN_UIC_HEADERS} ${KHOPPER_MAIN_RCC_SOURCES})
    target_link_libraries(khopper khopper_core ${QT_LIBRARIES} ${QT_PHONON_LIBRARY})
endif()
add_dependencies(khopper khopper_core)
set_target_properties(khopper PROPERTIES VERSION ${KHOPPER_VERSION})

install(TARGETS khopper DESTINATION "bin")
