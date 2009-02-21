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
 * @namespace Khopper
 * @brief All componets in this program
 */
#include "mainwindow.hpp"

#include <QApplication>
#include <QSettings>

extern "C" {
#include <avformat.h>
}

/// Main program
int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	av_register_all();

	QApplication::setWindowIcon( QIcon( ":/image/logo.svg" ) );
	QApplication::setOrganizationName( "FoolproofProject" );
	QApplication::setApplicationName( "Khopper" );
	QSettings::setDefaultFormat( QSettings::IniFormat );

	Khopper::MainWindow window;
	window.setWindowTitle( "Khopper" );
	window.resize( 640, 480 );
	window.show();

	return app.exec();
}
