#include <QApplication>

#include "mainwindow.hpp"

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	
	Khopper::MainWindow window;
	window.resize( 640, 480 );
	window.show();
	
	return app.exec();
}
