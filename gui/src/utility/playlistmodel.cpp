/**
 * @file playlistmodel.cpp
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
#include "playlistmodel.hpp"
#include "khopper/album.hpp"

#include <algorithm>

using namespace khopper::utility;
using khopper::album::PlayList;

PlayListModel::PlayListModel( QObject * parent ):
QAbstractItemModel( parent ),
list_() {
}

QModelIndex PlayListModel::index( int row, int column, const QModelIndex & parent ) const {
	if( row >= this->rowCount( parent ) ) {
		return QModelIndex();
	}
	if( column >= this->columnCount( parent ) ) {
		return QModelIndex();
	}
	return this->createIndex( row, column, const_cast< void * >( static_cast< const void * >( this->list_[row].get() ) ) );
}

QModelIndex PlayListModel::parent( const QModelIndex & /*index*/ ) const {
	return QModelIndex();
}

void PlayListModel::changeTextCodec( const QModelIndex & index, QTextCodec * codec ) {
	this->list_[index.row()]->setTextCodec( codec );
}

void PlayListModel::append( const PlayList & playList ) {
	if( playList.empty() ) {
		return;
	}
	int start = this->list_.size();
	int end = this->list_.size() + playList.size() - 1;
	this->beginInsertRows( QModelIndex(), start, end );
	this->list_.append( playList );
	this->endInsertRows();
}

void PlayListModel::remove( QModelIndexList indexes ) {
	std::sort( indexes.begin(), indexes.end(), []( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() > r.row();
	} );
	std::for_each( indexes.begin(), indexes.end(), [&]( const QModelIndex & i ) {
		this->beginRemoveRows( QModelIndex(), i.row(), i.row() );
		this->list_.erase( std::next( this->list_.begin(), i.row() ) );
		this->endRemoveRows();
	} );
}

int PlayListModel::columnCount( const QModelIndex & parent ) const {
	return parent.isValid() ? 0 : 4;
}

bool PlayListModel::setData( const QModelIndex & index, const QVariant & value, int role ) {
	if( !index.isValid() || !value.isValid() ) {
		return false;
	}
	switch( role ) {
	case Qt::EditRole:
		switch( index.column() ) {
		case 0:
			this->list_[index.row()]->setTitle( value.toString() );
			break;
		case 1:
			this->list_[index.row()]->setArtist( value.toString() );
			break;
		case 2:
		{
			auto album = this->list_[index.row()]->getAlbum().lock();
			album->setTitle( value.toString() );
			break;
		}
		case 3:
			this->list_[index.row()]->setDuration( value.value< album::Timestamp >() );
			break;
		}
		break;
	default:
		return false;
	}
	return true;
}

QVariant PlayListModel::data( const QModelIndex & index, int role ) const {
	if( !index.isValid() ) {
		return QVariant();
	}
	switch( role ) {
	case Qt::DisplayRole:
		switch( index.column() ) {
		case 0:
			return this->list_[index.row()]->getTitle();
		case 1:
			return this->list_[index.row()]->getArtist();
		case 2:
		{
			auto album = this->list_[index.row()]->getAlbum().lock();
			return album->getTitle();
		}
		case 3:
			return this->list_[index.row()]->getDuration().toString();
		default:
			return QVariant();
		}
		break;
	default:
		return QVariant();
	}
}

const PlayList & PlayListModel::getPlayList() const {
	return this->list_;
}

QVariant PlayListModel::headerData( int section, Qt::Orientation orientation, int role ) const {
	switch( role ) {
	case Qt::DisplayRole:
		switch( orientation ) {
		case Qt::Horizontal:
			switch( section ) {
			case 0:
				return tr( "Title" );
			case 1:
				return tr( "Artist" );
			case 2:
				return tr( "Album Title" );
			case 3:
				return tr( "Duration" );
			default:
				return QVariant();
			}
		default:
			return QVariant();
		}
		break;
	default:
		return QVariant();
	}
}

int PlayListModel::rowCount( const QModelIndex & parent ) const {
	return parent.isValid() ? 0 : this->list_.size();
}
