INSTALL_PREFIX = /usr/local

TEMPLATE = subdirs
SUBDIRS = core main plugins
CONFIG += ordered debug_and_release

unix {
	application.path = $${INSTALL_PREFIX}/bin
	application.files = build/release/khopper
	library.path = $${INSTALL_PREFIX}/lib
	library.files = build/release/libk_core.so.0.1.80
	library.extra =	\
		ln -s $${INSTALL_PREFIX}/lib/libk_core.so.0.1.80 $${INSTALL_PREFIX}/lib/libk_core.so.0.1;	\
		ln -s $${INSTALL_PREFIX}/lib/libk_core.so.0.1.80 $${INSTALL_PREFIX}/lib/libk_core.so.0;	\
		ln -s $${INSTALL_PREFIX}/lib/libk_core.so.0.1.80 $${INSTALL_PREFIX}/lib/libk_core.so;
	plugins.path = $${INSTALL_PREFIX}/lib/khopper/plugins
	plugins.files = build/release/plugins/*.so
	INSTALLS += application library plugins
}
