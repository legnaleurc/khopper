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
#include "pluginmanager.hpp"
#include "abstractplugin.hpp"

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

using namespace khopper::plugin;

PluginManager::PluginManager():
searchPaths_(),
loadedPlugins_() {
	// initialize search paths

	// first search binary related paths, for build time testing
	QDir tmp( qApp->applicationDirPath() );
#ifdef _MSC_VER
	// hack for MSVC
# ifdef _DEBUG
	if( tmp.cd( "../plugins/Debug" ) ) {
# else
	if( tmp.cd( "../plugins/Release" ) ) {
# endif
		this->searchPaths_.push_back( tmp );
	}
	tmp = qApp->applicationDirPath();
#endif

	if( tmp.cd( "../lib/plugins" ) ) {
		this->searchPaths_.push_back( tmp );
	}

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

PluginManager::~PluginManager() {
	this->unloadPlugins();
}

void PluginManager::reloadPlugins() {
	this->unloadPlugins();

	foreach( QFileInfo fileInfo, this->getPluginsFileInfo_() ) {
		qDebug() << fileInfo.absoluteFilePath();
		QPluginLoader loader( fileInfo.absoluteFilePath() );
		AbstractPlugin * pPlugin = dynamic_cast< AbstractPlugin * >( loader.instance() );
		if( pPlugin == NULL ) {
			if( loader.isLoaded() ) {
				emit this->errorOccured( QObject::tr( "Invalid plugin" ), QObject::tr( "This plugin: %1 is not valid for Khopper." ).arg( fileInfo.absoluteFilePath() ) );
			} else {
				emit this->errorOccured( QObject::tr( "Plugin load error" ), loader.errorString() );
			}
			continue;
		}

		std::string id = pPlugin->getID().toStdString();
		std::map< std::string, AbstractPlugin * >::const_iterator it = this->loadedPlugins_.find( id );
		if( it == this->loadedPlugins_.end() ) {
			try {
				pPlugin->install( fileInfo );
			} catch( error::BaseError & e ) {
				emit this->errorOccured( QObject::tr( "Plugin install error" ), e.getMessage() );
				loader.unload();
				continue;
			}
			this->loadedPlugins_.insert( std::make_pair( id, pPlugin ) );
		} else {
			bool unloaded = loader.unload();
			qDebug() << "unload deprecated plugin:" << unloaded;
		}
	}
}

void PluginManager::unloadPlugins() {
	for( std::map< std::string, AbstractPlugin * >::iterator it = this->loadedPlugins_.begin(); it != this->loadedPlugins_.end(); ++it ) {
		it->second->uninstall();
	}
	this->loadedPlugins_.clear();
}

AbstractPlugin * PluginManager::getPluginInstance( const QString & name ) const {
	std::map< std::string, AbstractPlugin * >::const_iterator it = this->loadedPlugins_.find( name.toStdString() );
	if( it != this->loadedPlugins_.end() ) {
		return it->second;
	} else {
		return NULL;
	}
}

QFileInfoList PluginManager::getPluginsFileInfo_() const {
	QFileInfoList list;
	foreach( QDir d, this->searchPaths_ ) {
		list.append( d.entryInfoList( getPluginNameFilter(), QDir::Files ) );
	}
	return list;
}
