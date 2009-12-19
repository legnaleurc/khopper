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
#include "propertieswidget.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QSignalMapper>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtDebug>
#include <QtGui/QAction>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMenu>

#include <algorithm>

namespace {

	inline bool indexRowCompD( const QModelIndex & l, const QModelIndex & r ) {
		return l.row() > r.row();
	}

	struct HeaderData {
		HeaderData( const QByteArray & i, const QString & h ):
		id( i ), header( h ) {}
		QByteArray id;
		QString header;
	};

	typedef QList< HeaderData > HeaderDataList;

	inline HeaderDataList initHeaderList() {
		return HeaderDataList()
		<< HeaderData( "title", QObject::tr( "Title" ) )
		<< HeaderData( "artist", QObject::tr( "Artist" ) )
		<< HeaderData( "album", QObject::tr( "Album" ) )
		<< HeaderData( "duration", QObject::tr( "Duration" ) )
		<< HeaderData( "bit_rate", QObject::tr( "Bit Rate" ) )
		<< HeaderData( "sample_rate", QObject::tr( "Sample Rate" ) )
		<< HeaderData( "channels", QObject::tr( "Channels" ) )
		;
	}

	inline const HeaderDataList & getHeaderList() {
		static HeaderDataList hdl( initHeaderList() );
		return hdl;
	}

	QString displayHelper( const QVariant & v ) {
		if( v.canConvert( QVariant::String ) ) {
			return v.toString();
		} else if( v.canConvert( QVariant::Int ) ) {
			return QString::number( v.toInt() );
		} else if( v.canConvert< khopper::album::Timestamp >() ) {
			return v.value< khopper::album::Timestamp >().toString();
		} else if( !v.isValid() ) {
			qDebug() << v;
			return "!!!";
		} else {
			return "to be continue";
		}
	}

}

namespace khopper {
	namespace widget {

		SongList::SongList( QWidget * parent ):
		QTableView( parent ),
		model_( new QStandardItemModel( this ) ),
		contextMenu_( new QMenu( this ) ),
		propWidget_( new PropertiesWidget( this ) ),
		tracks_(),
		droppingFiles_() {
			// Set drag and drop
			this->setAcceptDrops( true );

			// Set selection behavior
			this->setSelectionBehavior( SelectRows );
			this->setEditTriggers( NoEditTriggers );

			this->setWordWrap( false );
			this->setShowGrid( false );

			// Set header
			QStringList headers;
			foreach( HeaderData hd, getHeaderList() ) {
				headers << hd.header;
			}
			this->model_->setHorizontalHeaderLabels( headers );

			// Set model
			this->setModel( this->model_ );
			QAction * delSong = new QAction( this );
			delSong->setShortcut( QKeySequence::Delete );
			this->addAction( delSong );
			connect( delSong, SIGNAL( triggered() ), this, SLOT( removeSelected_() ) );

			// Set context menu
			QMenu * codecs = new QMenu( tr( "Change Text Codec" ), this );

			QSignalMapper * sm = new QSignalMapper( this );

			// generate codec menu
			foreach( int mib, QTextCodec::availableMibs() ) {
				QAction * codec = new QAction( QTextCodec::codecForMib( mib )->name(), this );
				codecs->addAction( codec );
				connect( codec, SIGNAL( triggered() ), sm, SLOT( map() ) );
				sm->setMapping( codec, mib );
			}

			connect( sm, SIGNAL( mapped( int ) ), this, SLOT( changeTextCodec_( int ) ) );

			this->contextMenu_->addMenu( codecs );

			this->contextMenu_->addSeparator();

			QAction * properties = new QAction( tr( "Properties" ), this );
			connect( properties, SIGNAL( triggered() ), this, SLOT( propertiesHelper_() ) );
			this->contextMenu_->addAction( properties );

			this->contextMenu_->addSeparator();

			QAction * convert = new QAction( tr( "Convert" ), this );
			convert->setShortcut( Qt::CTRL + Qt::Key_Return );
			connect( convert, SIGNAL( triggered() ), this, SIGNAL( requireConvert() ) );
			this->addAction( convert );
			this->contextMenu_->addAction( convert );
		}

		void SongList::propertiesHelper_() {
			QModelIndexList selected = this->selectionModel()->selectedRows();
			if( selected.isEmpty() ) {
				// no track selected
				return;
			}
			this->propWidget_->exec( this->getSelectedTracks() );
		}

		void SongList::appendTracks( const std::vector< album::TrackSP > & tracks ) {
			this->tracks_.insert( this->tracks_.end(), tracks.begin(), tracks.end() );

			// get last row number
			int offset = this->model_->rowCount();
			// add all tracks
			for( std::size_t row = 0; row < tracks.size(); ++row ) {
				const std::size_t currentRow = row + offset;
				for( int col = 0; col < getHeaderList().size(); ++col ) {
					this->model_->setItem( currentRow, col, new QStandardItem( displayHelper( tracks[row]->get( getHeaderList()[col].id ) ) ) );
				}

				this->resizeRowToContents( currentRow );
			}
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

		void SongList::changeTextCodec_( int mib ) {
			QTextCodec * codec = QTextCodec::codecForMib( mib );
			QModelIndexList selected = this->selectionModel()->selectedRows();
			foreach( QModelIndex index, selected ) {
				album::TrackSP track( this->tracks_[index.row()] );
				track->setTextCodec( codec );

				// FIXME: generic
				this->model_->item( index.row(), 0 )->setText( track->get( "title" ).toString() );
				this->model_->item( index.row(), 1 )->setText( track->get( "artist" ).toString() );
				this->model_->item( index.row(), 2 )->setText( track->get( "album" ).toString() );
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
				this->droppingFiles_ = event->mimeData()->urls();
				QTimer::singleShot( 0, this, SLOT( dropFiles_() ) );
			}
			event->acceptProposedAction();
		}

		void SongList::dropFiles_() {
			emit fileDropped( this->droppingFiles_ );
		}

	}
}
