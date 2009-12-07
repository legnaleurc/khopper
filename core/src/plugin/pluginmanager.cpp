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

#include <QApplication>
#include <QtGlobal>
#include <QtDebug>
#include <QPluginLoader>

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
			searchPaths_(),
			loadedPlugins_(),
			loadedPanels_() {
				// initialize search paths
				QDir tmp( qApp->applicationDirPath() );
				if( tmp.cd( "../lib/plugins" ) ) {
					this->searchPaths_.push_back( tmp );
				}
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

				this->reloadPlugins();
			}

			void PluginManager::reloadPlugins() {
				this->loadedPlugins_.clear();
				this->loadedPanels_.clear();

				foreach( QString filePath, this->getPluginFiles_() ) {
					qDebug() << filePath;
					QPluginLoader loader( filePath );
					QObject * pInstance = loader.instance();
					if( pInstance ) {
						AbstractPlugin * pPlugin = dynamic_cast< AbstractPlugin * >( pInstance );
						if( pPlugin ) {
							std::string id = pPlugin->getID().toStdString();
							std::map< std::string, QObject * >::const_iterator it = this->loadedPlugins_.find( id );
							if( it == this->loadedPlugins_.end() ) {
								this->loadedPlugins_.insert( std::make_pair( id, pInstance ) );
								AbstractPanel * panel = qobject_cast< AbstractPanel * >( pInstance );
								if( panel ) {
									this->loadedPanels_.push_back( panel );
								}
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

			QObject * PluginManager::getPluginInstance( const QString & name ) const {
				std::map< std::string, QObject * >::const_iterator it = this->loadedPlugins_.find( name.toStdString() );
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
