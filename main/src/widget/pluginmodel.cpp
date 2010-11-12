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

using namespace khopper::widget;

PluginModel::PluginModel( QObject * parent ) : QAbstractTableModel( parent ) {
	this->setHeaderData( 0, Qt::Horizontal, "ID", Qt::DisplayRole );
	this->setHeaderData( 1, Qt::Horizontal, "Version", Qt::DisplayRole );
	this->setHeaderData( 2, Qt::Horizontal, "Path", Qt::DisplayRole );
	this->setHeaderData( 3, Qt::Horizontal, "Loaded", Qt::DisplayRole );
}

int PluginModel::columnCount( const QModelIndex & /*parent*/ ) const {
	return 4;
}

QVariant PluginModel::data( const QModelIndex & /*index*/, int /*role*/ ) const {
	return QVariant();
}

int PluginModel::rowCount( const QModelIndex & /*parent*/ ) const {
	return 0;
}
