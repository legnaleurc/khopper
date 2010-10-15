#include "playlistmodel.hpp"

using namespace khopper::widget;
using khopper::album::PlayList;

PlayListModel::PlayListModel( QObject * parent ):
QAbstractTableModel( parent ),
list_() {
}

void PlayListModel::append( const PlayList & playList ) {
	int start = this->list_.size();
	int end = this->list_.size() + playList.size() - 1;
	this->beginInsertRows( QModelIndex(), start, end );
	this->list_.append( playList );
	this->endInsertRows();
}

int PlayListModel::columnCount( const QModelIndex & parent ) const {
	return 4;
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
			return this->list_[index.row()]->getAlbum()->getTitle();
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
	return this->list_.size();
}
