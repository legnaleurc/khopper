file(READ $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/applications/khopper.desktop desktop)
string(REPLACE ";" "\;" desktop "${desktop}")
string(REPLACE "__ICON__" "${CMAKE_INSTALL_PREFIX}/share/pixmap/khopper.png" desktop "${desktop}")
file(WRITE $ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/applications/khopper.desktop ${desktop})
