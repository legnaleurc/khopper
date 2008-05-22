#include <QApplication>

#include "ui/MainWindow.hpp"

#include <vector>
#include <map>

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	
	Khopper::MainWindow window;
	window.show();
	
// 	std::vector< std::vector< std::string > > v;
// 	m["name"] = "Northern Lights";
// 	m["author"] = "Megumi Hayashibara";
// 	m["album"] = "Shaman King";
// 	v.push_back( m );
// 	m["name"] = "Shizuku";
// 	m["author"] = "Miwako Okuda";
// 	m["album"] = "GTO";
// 	v.push_back( m );
// 	m["name"] = "Alone";
// 	m["author"] = "Mikuni Shimokawa";
// 	m["album"] = "Saiyuki";
// 	v.push_back( m );
// 	
// 	window.setSongList( v );
	
	return app.exec();
}
