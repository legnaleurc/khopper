/**
 * @file pluginmanager.cpp
 * @author Wei-Cheng Pan
 *
 * Copyright (C) 2008 Wei-Cheng Pan <legnaleurc@gmail.com>
 *
 * This file is part of Khopper.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "plugin/pluginmanager.hpp"
#include "plugin/abstractplugin.hpp"
#include "plugin/abstractpanel.hpp"

#include <QtCore/QPluginLoader>
#include <QtDebug>
#include <QtGlobal>
#include <QtGui/QApplication>

namespace {

	inline const QStringList & getPluginNameFilter() {
#ifdef Q_OS_UNIX
		static QStringList f( "*.so" );
#elif defined(Q_OS_WIN32)
		static QStringList f( "*.dll" );
#else
#error
#endif
		return f;
	}

}

namespace khopper {

	namespace plugin {

		namespace private_ {

			PluginManager::PluginManager():
			QObject( 0 ),
			searchPaths_(),
			loadedPlugins_() {
				// initialize search paths

				// first search binary related paths, for build time testing
				QDir tmp( qApp->applicationDirPath() );
#ifdef _MSC_VER
				// hack for MSVC
# ifdef _DEBUG
				if( tmp.cd( "../plugins/Debug" ) ) {
					this->searchPaths_.push_back( tmp );
				}
# else
				if( tmp.cd( "../plugins/Release" ) ) {
					this->searchPaths_.push_back( tmp );
				}
# endif
#else
				if( tmp.cd( "../lib/plugins" ) ) {
					this->searchPaths_.push_back( tmp );
				}
#endif
				// second search personal settings
				tmp = QDir::home();
				if( tmp.cd( ".khopper/plugins" ) ) {
					this->searchPaths_.push_back( tmp );
				}
				tmp = QDir( "/usr/local/lib/khopper/plugins" );
				if( tmp.exists() ) {
					this->searchPaths_.push_back( tmp );
				}
				tmp = QDir( "/usr/lib/khopper/plugins" );
				if( tmp.exists() ) {
					this->searchPaths_.push_back( tmp );
				}
			}

			void PluginManager::reloadPlugins() {
				this->loadedPlugins_.clear();

				foreach( QString filePath, this->getPluginFiles_() ) {
					qDebug() << filePath;
					QPluginLoader loader( filePath );
					QObject * pInstance = loader.instance();
					if( pInstance ) {
						AbstractPlugin * pPlugin = dynamic_cast< AbstractPlugin * >( pInstance );
						if( pPlugin ) {
							std::string id = pPlugin->getID().toStdString();
							std::map< std::string, AbstractPlugin * >::const_iterator it = this->loadedPlugins_.find( id );
							if( it == this->loadedPlugins_.end() ) {
								try {
									pPlugin->install( QFileInfo( filePath ) );
								} catch( error::BaseError & e ) {
									qDebug() << "install error:" << e.what();
									continue;
								}
								this->loadedPlugins_.insert( std::make_pair( id, pPlugin ) );
							} else {
								bool unloaded = loader.unload();
								qDebug() << "unload deprecated plugin:" << unloaded;
							}
						}
					} else {
						qDebug() << loader.errorString();
					}
				}
			}

			void PluginManager::addPanel( AbstractPanel * panel ) {
				emit this->panelAdded( panel );
			}

			void PluginManager::removePanel( AbstractPanel * panel ) {
				emit this->panelRemoved( panel );
			}

			AbstractPlugin * PluginManager::getPluginInstance( const QString & name ) const {
				std::map< std::string, AbstractPlugin * >::const_iterator it = this->loadedPlugins_.find( name.toStdString() );
				if( it != this->loadedPlugins_.end() ) {
					return it->second;
				} else {
					return NULL;
				}
			}

			QStringList PluginManager::getPluginFiles_() const {
				QStringList list;
				foreach( QDir d, this->searchPaths_ ) {
					foreach( QString fileName, d.entryList( getPluginNameFilter(), QDir::Files ) ) {
						list << d.absoluteFilePath( fileName );
					}
				}
				return list;
			}

		}

	}

}
