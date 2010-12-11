/**
 * @file pluginmodel.hpp
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
#ifndef KHOPPER_WIDGET_PLUGINMODEL_HPP
#define KHOPPER_WIDGET_PLUGINMODEL_HPP

#include <QtCore/QAbstractTableModel>

namespace khopper {
	namespace widget {

		class PluginModel : public QAbstractTableModel {
		public:
			explicit PluginModel( QObject * parent );

			virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
			virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
			virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
			virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;
		};

	}
}

#endif
