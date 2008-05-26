#include <QApplication>

#include "ui/MainWindow.hpp"

#include <vector>
#include <map>

#include <cstdlib>
#include "unistd.h"
	
const char * const LIST = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string strFromRand() {
	std::string result;
	int len = rand() % 52;
	result.reserve( len );
	for( int i = 0; i < len; ++i ) {
		result.push_back( LIST[rand()%52] );
	}
	return result;
}

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	
	Khopper::MainWindow window;
	window.resize( 640, 480 );
	window.show();
	
	srand( getpid() );
	
	std::vector< Khopper::CUESheet::FieldType > songs;
	int len = rand() % 9 + 1;
	songs.reserve( len );
	for( int i = 0; i < len; ++i ) {
		Khopper::CUESheet::FieldType temp;
		for( int j = 0; j < Khopper::CUESheet::Track::SIZE; ++j ) {
			temp.push_back( strFromRand() );
		}
		songs.push_back( temp );
	}
	
	window.setSongList( songs );
	
	return app.exec();
}
