/**
 * @file songlist.cpp
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
#include "songlist.hpp"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QUrl>
#include <QFileInfo>
#include <QMenu>
#include <QAction>
#include <QSignalMapper>
#include <QTextCodec>
#include <QtDebug>

namespace {

	inline bool indexRowCompD( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() > r.row();
	}

}

namespace khopper {

	namespace widget {

		SongList::SongList( QWidget * parent ):
		QTableView( parent ),
		model_( new QStandardItemModel( this ) ),
		contextMenu_( new QMenu( this ) ),
		tracks_() {
			// Set drag and drop
			this->setAcceptDrops( true );

			// Set selection behavior
			this->setSelectionBehavior( SelectRows );
			this->setEditTriggers( DoubleClicked );

			// Set header
			QStringList headers;
			headers << "Title";
			headers << "Artist";
			headers << "Album";
			headers << "Duration";
			headers << "Bit Rate";
			headers << "Channels";
			headers << "Sample Rate";
			this->model_->setHorizontalHeaderLabels( headers );

			// Set model
			this->setModel( this->model_ );
			connect( this->model_, SIGNAL( itemChanged( QStandardItem * ) ), this, SLOT( editTrackField_( QStandardItem * ) ) );
			QAction * delSong = new QAction( this );
			delSong->setShortcut( QKeySequence::Delete );
			this->addAction( delSong );
			connect( delSong, SIGNAL( triggered() ), this, SLOT( removeSelected_() ) );

			// Set context menu
			QMenu * codec = new QMenu( tr( "Change Text Codec" ), this );

			QSignalMapper * sm = new QSignalMapper( this );

			QAction * big5 = new QAction( tr( "Big5" ), this );
			codec->addAction( big5 );
			connect( big5, SIGNAL( triggered() ), sm, SLOT( map() ) );
			sm->setMapping( big5, "Big5" );
			QAction * sjis = new QAction( tr( "Shift-JIS" ), this );
			codec->addAction( sjis );
			connect( sjis, SIGNAL( triggered() ), sm, SLOT( map() ) );
			sm->setMapping( sjis, "Shift-JIS" );
			QAction * utf8 = new QAction( tr( "UTF-8" ), this );
			codec->addAction( utf8 );
			connect( utf8, SIGNAL( triggered() ), sm, SLOT( map() ) );
			sm->setMapping( utf8, "UTF-8" );

			connect( sm, SIGNAL( mapped( const QString & ) ), this, SLOT( changeTextCodec_( const QString & ) ) );

			this->contextMenu_->addMenu( codec );
			this->contextMenu_->addSeparator();

			QAction * convert = new QAction( tr( "Convert" ), this );
			convert->setShortcut( Qt::CTRL + Qt::Key_Return );
			connect( convert, SIGNAL( triggered() ), this, SIGNAL( requireConvert() ) );
			this->addAction( convert );
			this->contextMenu_->addAction( convert );
		}

		void SongList::appendTracks( const std::vector< album::TrackSP > & tracks ) {
			this->tracks_.insert( this->tracks_.end(), tracks.begin(), tracks.end() );

			// get last row number
			int offset = this->model_->rowCount();
			// add all tracks
			for( std::size_t row = 0; row < tracks.size(); ++row ) {
				this->model_->setItem( row + offset, 0, new QStandardItem( tracks[row]->getTitle() ) );
				this->model_->setItem( row + offset, 1, new QStandardItem( tracks[row]->getArtist() ) );
				this->model_->setItem( row + offset, 2, new QStandardItem( tracks[row]->getAlbum() ) );

				// fields should not be editable
				this->model_->setItem( row + offset, 3, new QStandardItem( QString::fromStdWString( tracks[row]->getDuration().toStdWString() ) ) );
				this->model_->item( row + offset, 3 )->setEditable( false );
				this->model_->setItem( row + offset, 4, new QStandardItem( QString::number( tracks[row]->getBitRate() ) ) );
				this->model_->item( row + offset, 4 )->setEditable( false );
				this->model_->setItem( row + offset, 5, new QStandardItem( QString::number( tracks[row]->getSampleRate() ) ) );
				this->model_->item( row + offset, 5 )->setEditable( false );
				this->model_->setItem( row + offset, 6, new QStandardItem( QString::number( tracks[row]->getChannels() ) ) );
				this->model_->item( row + offset, 6 )->setEditable( false );
			}
		}

		const std::vector< album::TrackSP > & SongList::getTracks() const {
			return this->tracks_;
		}

		std::vector< album::TrackSP > SongList::getSelectedTracks() const {
			QModelIndexList selected = this->selectionModel()->selectedRows( 0 );
			std::sort( selected.begin(), selected.end(), ::indexRowCompD );
			std::vector< album::TrackSP > result( selected.size() );

			for( std::size_t i = 0; i < result.size(); ++i ) {
				result[i] = this->tracks_.at( selected[i].row() );
			}

			return result;
		}

		void SongList::changeTextCodec_( const QString & name ) {
			QTextCodec * codec = QTextCodec::codecForName( name.toAscii() );
			QModelIndexList selected = this->selectionModel()->selectedRows();
			foreach( QModelIndex index, selected ) {
				album::TrackSP track( this->tracks_[index.row()] );
				track->setTextCodec( codec );

				this->model_->item( index.row(), 0 )->setText( track->getTitle() );
				this->model_->item( index.row(), 1 )->setText( track->getArtist() );
				this->model_->item( index.row(), 2 )->setText( track->getAlbum() );
			}
		}

		void SongList::removeSelected_() {
			QModelIndexList selected = this->selectionModel()->selectedRows();
			std::sort( selected.begin(), selected.end(), ::indexRowCompD );
			foreach( QModelIndex index, selected ) {
				this->tracks_.erase( this->tracks_.begin() + index.row() );
				this->model_->removeRow( index.row() );
			}
			this->selectionModel()->clear();
		}

		void SongList::editTrackField_( QStandardItem * item ) {
			album::TrackSP track = this->tracks_.at( item->row() );
			switch( item->column() ) {
			case 0:
				track->setTitle( item->text() );
				break;
			case 1:
				track->setArtist( item->text() );
				break;
			case 2:
				track->setAlbum( item->text() );
				break;
			default:
				// other fields should not be editable
				;
			}
		}

		void SongList::contextMenuEvent( QContextMenuEvent * event ) {
			this->contextMenu_->exec( event->globalPos() );
		}

		void SongList::dragEnterEvent( QDragEnterEvent * event ) {
			if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
				event->acceptProposedAction();
			}
		}

		void SongList::dragMoveEvent( QDragMoveEvent * event ) {
			if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
				event->acceptProposedAction();
			}
		}

		void SongList::dropEvent( QDropEvent * event ) {
			if( event->mimeData()->hasUrls() ) {
				QStringList local;
				foreach( QUrl url, event->mimeData()->urls() ) {
					local.push_back( url.toLocalFile() );
				}
				emit dropFile( local );
			}
			event->acceptProposedAction();
		}

	}

}
