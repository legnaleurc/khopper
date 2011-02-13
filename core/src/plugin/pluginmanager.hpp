/**
 * @file pluginmanager.hpp
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
#ifndef KHOPPER_PLUGIN_PLUGINMANAGER_HPP
#define KHOPPER_PLUGIN_PLUGINMANAGER_HPP

#include "error.hpp"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QDir>

#include <string>
#include <list>
#include <map>

namespace khopper {

	namespace plugin {

		class AbstractPanel;
		class AbstractPlugin;

		/**
		 * @ingroup Plugins
		 * @brief Private plugin manager
		 */
		class PluginManager : public QAbstractItemModel {
			Q_OBJECT

		public:
			/// default constructor
			PluginManager();
			virtual ~PluginManager();

			virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
			virtual QModelIndex parent( const QModelIndex & index ) const;
			virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
			virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
			virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
			virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

			/// get plugin instance by name
			AbstractPlugin * getPluginInstance( const QString & name ) const;
			const std::list< QDir > & getSearchPaths() const {
				return this->searchPaths_;
			}

			/**
			 * @brief reload all plugins
			 *
			 * Will discard duplicated plugins.
			 */
			void reloadPlugins();
			void unloadPlugins();

		signals:
			void errorOccured( const QString & title, const QString & message );

		private:
			QFileInfoList getPluginsFileInfo_() const;

			std::list< QDir > searchPaths_;
			std::map< std::string, AbstractPlugin * > loadedPlugins_;
		};

	}

}

#endif
