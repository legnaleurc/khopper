/**
 * @file playlistmodel.hpp
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
#ifndef KHOPPER_WIDGET_PLAYLISTMODEL_HPP
#define KHOPPER_WIDGET_PLAYLISTMODEL_HPP

#include "khopper/playlist.hpp"

#include <QtCore/QAbstractItemModel>

namespace khopper {
	namespace widget {
		class PlayListModel : public QAbstractItemModel {
		public:
			explicit PlayListModel( QObject * parent );

			void append( const album::PlayList & playList );
			void changeTextCodec( const QModelIndex & index, QTextCodec * codec );
			virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
			virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
			const album::PlayList & getPlayList() const;
			virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
			virtual QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
			virtual QModelIndex parent( const QModelIndex & index ) const;
			virtual bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
			void remove( QModelIndexList indexes );
			virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

		private:
			album::PlayList list_;
		};
	}
}

#endif
