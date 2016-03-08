# gui

include(FeatureSummary)

if(MSVC)
    set(QT_USE_QTMAIN TRUE)
endif()
find_package(Qt4 4.7 QUIET COMPONENTS Phonon)
set_package_properties(Qt4
    PROPERTIES
    TYPE RECOMMENDED
    PURPOSE "GUI needs Qt Phonon to play music")

if(NOT QT_PHONON_FOUND)
    return()
endif()

include(${QT_USE_FILE})
include(KhopperUtility)

file(GLOB_RECURSE KHOPPER_GUI_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.hpp)
file(GLOB_RECURSE KHOPPER_GUI_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE KHOPPER_GUI_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.ui)
group_sources("src")

include_directories(
"src/utility"
"src/widget"
"${CMAKE_CURRENT_BINARY_DIR}"
"${CMAKE_SOURCE_DIR}/core/include"
"${QT_QTGUI_INCLUDE_DIR}"
"${QT_PHONON_INCLUDE_DIR}")

find_moc_headers(KHOPPER_GUI_MOC_HEADERS ${KHOPPER_GUI_HEADERS})
qt4_wrap_cpp(KHOPPER_GUI_MOC_SOURCES ${KHOPPER_GUI_MOC_HEADERS})
qt4_wrap_ui(KHOPPER_GUI_UIC_HEADERS ${KHOPPER_GUI_FORMS})
qt4_add_resources(KHOPPER_GUI_RCC_SOURCES main.qrc)

source_group("Generated Files" FILES ${KHOPPER_GUI_MOC_SOURCES} ${KHOPPER_GUI_RCC_SOURCES} ${KHOPPER_GUI_UIC_HEADERS})
source_group("Resource Files" REGULAR_EXPRESSION .*\\.rc)

set(KHOPPER_GUI_COMPILE_UNITS ${KHOPPER_GUI_HEADERS} ${KHOPPER_GUI_SOURCES} ${KHOPPER_GUI_MOC_SOURCES} ${KHOPPER_GUI_UIC_HEADERS} ${KHOPPER_GUI_RCC_SOURCES})

if(MSVC)
add_executable(khopper_gui WIN32 "main.rc" ${KHOPPER_GUI_COMPILE_UNITS})
else()
add_executable(khopper_gui ${KHOPPER_GUI_COMPILE_UNITS})
endif()
target_link_libraries(khopper_gui khopper ${QT_LIBRARIES})
add_dependencies(khopper_gui khopper)
set_target_properties(khopper_gui PROPERTIES VERSION ${KHOPPER_VERSION})

install(TARGETS khopper_gui DESTINATION "bin")
if(UNIX)
install(FILES share/applications/khopper.desktop DESTINATION "share/applications")
install(FILES share/pixmap/logo.png DESTINATION "share/pixmaps" RENAME "khopper.png")
install(SCRIPT postinstall.cmake)
endif()
