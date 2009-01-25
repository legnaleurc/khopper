#include "mainwindow.hpp"

#include <QApplication>

extern "C" {
#include <avformat.h>
}

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	av_register_all();
	
	Khopper::MainWindow window;
	window.resize( 640, 480 );
	window.show();
	
	return app.exec();
}
