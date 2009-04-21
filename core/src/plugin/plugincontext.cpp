/**
 * @file plugincontext.cpp
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
#include "plugin/plugincontext.hpp"

#include <QApplication>
#include <QLibraryInfo>

namespace {

	inline std::list< QDir > initPaths() {
		std::list< QDir > paths;
		QDir tmp( qApp->applicationDirPath() );

		if( tmp.cd( "plugins" ) ) {
			paths.push_back( tmp );
		}
		tmp = QDir( QLibraryInfo::location( QLibraryInfo::PluginsPath ) );
		if( tmp.exists() ) {
			paths.push_back( tmp );
		}

		return paths;
	}

}

namespace khopper {

	namespace plugin {

		const std::list< QDir > & PluginContext::getPaths_() {
			static std::list< QDir > p = initPaths();
			return p;
		}

		QStringList PluginContext::getList() {
			QStringList list;
			foreach( QDir d, getPaths_() ) {
				foreach( QString fileName, d.entryList( getFilter_(), QDir::Files ) ) {
					list << d.absoluteFilePath( fileName );
				}
			}
			return list;
		}

		QObject * PluginContext::load( QString name ) {
			QObject * result = NULL;
			foreach( QDir d, getPaths_() ) {
				if( d.exists( name ) ) {
					QPluginLoader pl( d.absoluteFilePath( toRealName_( name ) ) );
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