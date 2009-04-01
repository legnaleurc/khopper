/**
 * @file linux.cpp
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
#include "os.hpp"
#include "error.hpp"

#include <QRegExp>
#include <QApplication>
#include <QPluginLoader>

namespace khopper {

	namespace os {

		QString join( const QString & front, const QString & back ) {
			// kill all tail '/' in front
			// kill all lead '/' in back
			// return front + '/' + back
			QRegExp fp( "(.*)/*" );
			QRegExp bp( "/*(.*)" );

			if( fp.exactMatch( front ) && bp.exactMatch( back ) ) {
				return fp.cap( 1 ) + "/" + bp.cap( 1 );
			} else {
				return "";
			}
		}

	}

	namespace plugin {

		PluginContext::PluginContext():
		d_( qApp->applicationDirPath() ) {
			this->d_.cd( "plugins" );
		}

		const QDir & PluginContext::getDir() const {
			return this->d_;
		}

		QObject * PluginContext::load( QString name ) const {
			QPluginLoader pl( this->d_.absoluteFilePath( name.prepend( "lib" ).append( ".so" ) ) );
			QObject * tmp = pl.instance();
			if( !tmp ) {
				throw Error< RunTime >( pl.errorString().toStdString() );
			}
			return tmp;
		}

	}

}
