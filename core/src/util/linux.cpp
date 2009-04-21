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
#include "util/os.hpp"
#include "util/error.hpp"

#include <QRegExp>
#include <QApplication>
#include <QPluginLoader>
#include <QLibraryInfo>

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
		d_() {
			QDir tmp( qApp->applicationDirPath() );
			if( tmp.cd( "plugins" ) ) {
				this->d_.push_back( tmp );
			}
			tmp = QDir( QLibraryInfo::location( QLibraryInfo::PluginsPath ) );
			if( tmp.exists() ) {
				this->d_.push_back( tmp );
			}
		}

		QStringList PluginContext::getPluginList() const {
			QStringList list;
			foreach( QDir d, this->d_ ) {
				foreach( QString fileName, d.entryList( QStringList( "*.so" ), QDir::Files ) ) {
					list << d.absoluteFilePath( fileName );
				}
			}
			return list;
		}

		QObject * PluginContext::load( QString name ) const {
			QObject * result = NULL;
			foreach( QDir d, this->d_ ) {
				name.prepend( "lib" ).append( ".so" );
				if( d.exists( name ) ) {
					QPluginLoader pl( d.absoluteFilePath( name ) );
					QObject * tmp = pl.instance();
					if( !tmp ) {
						throw error::RunTimeError( pl.errorString() );
					}
					result = tmp;
					break;
				}
			}
			if( !result ) {
				throw error::IOError( "Find no such plugin." );
			} else {
				return result;
			}
		}

	}

}
