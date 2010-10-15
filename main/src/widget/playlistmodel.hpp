#ifndef KHOPPER_WIDGET_PLAYLISTMODEL_HPP
#define KHOPPER_WIDGET_PLAYLISTMODEL_HPP

#include "khopper/playlist.hpp"

#include <QtCore/QAbstractTableModel>

namespace khopper {
	namespace widget {
		class PlayListModel : public QAbstractTableModel {
		public:
			explicit PlayListModel( QObject * parent );

			void append( const album::PlayList & playList );
			virtual int columnCount( const QModelIndex & parent = QModelIndex() ) const;
			virtual QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
			const album::PlayList & getPlayList() const;
			virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
			virtual int rowCount( const QModelIndex & parent = QModelIndex() ) const;

		private:
			album::PlayList list_;
		};
	}
}

#endif
