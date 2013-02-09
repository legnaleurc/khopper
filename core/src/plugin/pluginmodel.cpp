/**
 * @file pluginmodel.cpp
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
#include "pluginmodel.hpp"

#include <algorithm>
#include <functional>

#include <QtCore/QPluginLoader>
#include <QtCore/QtDebug>
#include <QtCore/QtGlobal>
#include <QtGui/QApplication>

#include "abstractplugin.hpp"
#include "baseerror.hpp"
#include "runtimeerror.hpp"


namespace khopper {
namespace plugin {

class PluginModel::Private {
public:
	typedef std::vector< AbstractPlugin * > PluginListType;

	Private();

	QFileInfoList getPluginsFileInfo() const;

	std::list< QDir > searchPaths;
	PluginListType plugins;
};

PluginModel * self = nullptr;

}
}

using khopper::error::BaseError;
using khopper::error::RunTimeError;
using khopper::plugin::PluginModel;

PluginModel::Private::Private():
searchPaths(),
plugins() {
	if( self ) {
		throw RunTimeError( QObject::tr( "PluginModel has been initialized" ), __FILE__, __LINE__ );
	}
}

QFileInfoList PluginModel::Private::getPluginsFileInfo() const {
	QFileInfoList list;
	std::for_each( this->searchPaths.begin(), this->searchPaths.end(), [&]( const QDir & d ) {
		// FIXME: use Qt macro because of the STUPID problem of VC10 about nested capturing
		foreach( QFileInfo fi, d.entryInfoList( QDir::Files, QDir::Name ) ) {
			if( QLibrary::isLibrary( fi.fileName() ) ) {
				list.push_back( fi );
			}
		}
	} );
	return list;
}

PluginModel & PluginModel::instance() {
	if( !self ) {
		throw RunTimeError( QObject::tr( "PluginModel has not been initialized yet" ), __FILE__, __LINE__ );
	}
	return *self;
}

PluginModel::PluginModel():
QAbstractItemModel( 0 ),
p_( new Private ) {
	self = this;

	// initialize search paths

	// first search binary related paths, for build time testing
	QDir tmp( qApp->applicationDirPath() );
#ifdef CMAKE_INTDIR
	// hack for MSVC
	if( tmp.cd( "../plugins/" CMAKE_INTDIR ) ) {
		this->p_->searchPaths.push_back( tmp );
	}
	tmp = qApp->applicationDirPath();
#endif
	if( tmp.cd( "../lib" ) ) {
		this->p_->searchPaths.push_back( tmp );
	}

	// second search personal settings
	tmp = QDir::home();
	if( tmp.cd( ".khopper/plugins" ) ) {
		this->p_->searchPaths.push_back( tmp );
	}
	// last search system-wide settings
	tmp = QDir( "/usr/local/lib/khopper/plugins" );
	if( tmp.exists() ) {
		this->p_->searchPaths.push_back( tmp );
	}
	tmp = QDir( "/usr/lib/khopper/plugins" );
	if( tmp.exists() ) {
		this->p_->searchPaths.push_back( tmp );
	}
}

PluginModel::~PluginModel() {
	this->unloadPlugins();
	self = nullptr;
}

QModelIndex PluginModel::index( int row, int column, const QModelIndex & parent ) const {
	if( row >= this->rowCount( parent ) || column >= this->columnCount( parent ) ) {
		return QModelIndex();
	}
	return this->createIndex( row, column, this->p_->plugins[row] );
}

QModelIndex PluginModel::parent( const QModelIndex & /*index*/ ) const {
	return QModelIndex();
}

int PluginModel::rowCount( const QModelIndex & parent ) const {
	return parent.isValid() ? 0 : this->p_->plugins.size();
}

int PluginModel::columnCount( const QModelIndex & parent ) const {
	return parent.isValid() ? 0 : 4;
}

QVariant PluginModel::data( const QModelIndex & index, int role ) const {
	if( !index.isValid() ) {
		return QVariant();
	}
	std::map< std::string, AbstractPlugin * >::const_iterator it;
	switch( role ) {
	case Qt::DisplayRole:
		switch( index.column() ) {
		case 0:
			return this->p_->plugins[index.row()]->isInstalled();
			break;
		case 1:
			return this->p_->plugins[index.row()]->getID();
			break;
		case 2:
			return this->p_->plugins[index.row()]->getVersion();
			break;
		case 3:
			return this->p_->plugins[index.row()]->getFileInfo().absoluteFilePath();
			break;
		default:
			return QVariant();
		}
		break;
	default:
		return QVariant();
	}
}

QVariant PluginModel::headerData( int section, Qt::Orientation orientation, int role ) const {
	switch( role ) {
	case Qt::DisplayRole:
		switch( orientation ) {
		case Qt::Horizontal:
			switch( section ) {
			case 0:
				return tr( "Loaded" );
			case 1:
				return tr( "ID" );
			case 2:
				return tr( "Version" );
			case 3:
				return tr( "Path" );
			default:
				return QVariant();
			}
			break;
		default:
			return QVariant();
		}
		break;
	default:
		return QVariant();
	}
}

void PluginModel::reloadPlugins() {
	this->unloadPlugins();
	this->refreshPlugins();

	std::for_each( this->p_->plugins.begin(), this->p_->plugins.end(), [&]( const Private::PluginListType::value_type & pPlugin ) {
		try {
			pPlugin->install();
		} catch( BaseError & e ) {
			emit this->errorOccured( QObject::tr( "Plugin install error" ), e.getMessage() );
			return;
		}
	} );
}

void PluginModel::unloadPlugins() {
	for( Private::PluginListType::iterator it = this->p_->plugins.begin(); it != this->p_->plugins.end(); ++it ) {
		if( *it ) {
			( *it )->uninstall();
		}
	}
	this->p_->plugins.clear();
}

void PluginModel::loadPlugin( const QModelIndex & index ) {
	if( !index.isValid() || !this->p_->plugins[index.row()] || this->p_->plugins[index.row()]->isInstalled() ) {
		return;
	}
	try {
		this->p_->plugins[index.row()]->install();
	} catch( BaseError & e ) {
		emit this->errorOccured( QObject::tr( "Plugin install error" ), e.getMessage() );
	}
}

void PluginModel::unloadPlugin( const QModelIndex & index ) {
	if( !index.isValid() || !this->p_->plugins[index.row()] || !this->p_->plugins[index.row()]->isInstalled() ) {
		return;
	}
	try {
		this->p_->plugins[index.row()]->uninstall();
	} catch( BaseError & e ) {
		emit this->errorOccured( QObject::tr( "Plugin uninstall error" ), e.getMessage() );
	}
}

void PluginModel::refreshPlugins() {
	QFileInfoList pfi( this->p_->getPluginsFileInfo() );
	std::for_each( pfi.begin(), pfi.end(), [&]( const QFileInfo & fileInfo ) {
		qDebug() << fileInfo.absoluteFilePath();
		QPluginLoader loader( fileInfo.absoluteFilePath() );
		AbstractPlugin * pPlugin = dynamic_cast< AbstractPlugin * >( loader.instance() );
		if( !pPlugin ) {
			if( loader.isLoaded() ) {
				emit this->errorOccured( QObject::tr( "Invalid plugin" ), QObject::tr( "This plugin: %1 is not valid for Khopper." ).arg( fileInfo.absoluteFilePath() ) );
			} else {
				emit this->errorOccured( QObject::tr( "Plugin load error" ), loader.errorString() );
			}
			return;
		}

		auto it = std::find_if( this->p_->plugins.begin(), this->p_->plugins.end(), [pPlugin]( const AbstractPlugin * that )->bool {
			return that->getID() == pPlugin->getID();
		} );
		if( it != this->p_->plugins.end() ) {
			// if find deprecated plugin, unload it
			if( *it != loader.instance() ) {
				// NOTE: maybe unsafe with multiple inheritance
				bool unloaded = loader.unload();
				qDebug() << "unload deprecated plugin:" << unloaded;
			} else {
				qDebug() << "identical instance: ignored";
			}
		} else {
			pPlugin->setFileInfo( fileInfo );
			// if anything is right, add it to list
			this->p_->plugins.push_back( pPlugin );
		}
	} );
}
