/**
 * @file main.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * Khopper is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * Khopper is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/**
 * @namespace khopper
 * @brief All componets in this program
 */
#include "mainwindow.hpp"
#include "util/text.hpp"

#include <QApplication>
#include <QSettings>
#include <QTextCodec>

/// Main program
int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );

	QApplication::setWindowIcon( QIcon( ":/image/logo.svg" ) );
	QApplication::setOrganizationName( "FoolproofProject" );
	QApplication::setApplicationName( "Khopper" );
	QApplication::setApplicationVersion( KHOPPER_STRINGIZE(KHOPPER_VERSION) );
	QSettings::setDefaultFormat( QSettings::IniFormat );
	QTextCodec::setCodecForCStrings( QTextCodec::codecForName( "UTF-8" ) );
	QTextCodec::setCodecForTr( QTextCodec::codecForName( "UTF-8" ) );

	khopper::widget::MainWindow window;
	window.setWindowTitle( "Khopper" );
	window.resize( 640, 480 );
	window.show();

	QList< QUrl > tmp;
	foreach( QString path, QApplication::arguments().mid( 1 ) ) {
		tmp.push_back( path );
		if( tmp.back().scheme().isEmpty() ) {
			tmp.back().setScheme( "file" );
		}
	}

	window.open( tmp );

	return app.exec();
}
