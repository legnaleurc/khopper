/**
 * @file playlistview.cpp
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
#include "playlistview.hpp"

#include "khopper/playlist.hpp"

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

using namespace khopper::widget;
using khopper::album::PlayList;

PlayListView::PlayListView( QWidget * parent ):
QTableView( parent ),
contextMenu_( new QMenu( this ) ),
droppingFiles_() {
	// Set drag and drop
	this->setAcceptDrops( true );

	// Set model
	QAction * delSong = new QAction( this );
	delSong->setShortcut( QKeySequence::Delete );
	this->addAction( delSong );
	connect( delSong, SIGNAL( triggered() ), this, SLOT( removeSelectedTracks() ) );

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
	connect( properties, SIGNAL( triggered() ), this, SIGNAL( requireProperty() ) );
	this->contextMenu_->addAction( properties );

	this->contextMenu_->addSeparator();

	QAction * convert = new QAction( tr( "Convert" ), this );
	convert->setShortcut( Qt::CTRL + Qt::Key_Return );
	connect( convert, SIGNAL( triggered() ), this, SIGNAL( requireConvert() ) );
	this->addAction( convert );
	this->contextMenu_->addAction( convert );
}

//void PlayListView::changeTextCodec_( int mib ) {
//	QTextCodec * codec = QTextCodec::codecForMib( mib );
//	QModelIndexList selected = this->selectionModel()->selectedRows();
//	foreach( QModelIndex index, selected ) {
//		album::TrackSP track( this->tracks_[index.row()] );
//		track->setTextCodec( codec );
//
//		// FIXME: generic
//		this->model_->item( index.row(), 0 )->setText( track->getTitle() );
//		this->model_->item( index.row(), 1 )->setText( track->getArtist() );
//		this->model_->item( index.row(), 2 )->setText( track->getAlbum()->getTitle() );
//	}
//}

//void PlayListView::removeSelectedTracks() {
//	QModelIndexList selected = this->selectionModel()->selectedRows();
//	std::sort( selected.begin(), selected.end(), ::indexRowCompD );
//	foreach( QModelIndex index, selected ) {
//		this->tracks_.erase( this->tracks_.begin() + index.row() );
//		this->model_->removeRow( index.row() );
//	}
//	this->selectionModel()->clear();
//}

void PlayListView::contextMenuEvent( QContextMenuEvent * event ) {
	QModelIndex index( this->indexAt( event->pos() ) );
	if( index.isValid() ) {
		this->contextMenu_->exec( event->globalPos() );
	}
}

void PlayListView::dragEnterEvent( QDragEnterEvent * event ) {
	if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
		event->acceptProposedAction();
	}
}

void PlayListView::dragMoveEvent( QDragMoveEvent * event ) {
	if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
		event->acceptProposedAction();
	}
}

void PlayListView::dropEvent( QDropEvent * event ) {
	if( event->mimeData()->hasUrls() ) {
		this->droppingFiles_ = event->mimeData()->urls();
		QTimer::singleShot( 0, this, SLOT( dropFiles_() ) );
	}
	event->acceptProposedAction();
}

void PlayListView::mouseDoubleClickEvent( QMouseEvent * event ) {
	emit this->requirePlay();
	qDebug() << "Double Clicked Item:" << this->rowAt( event->y() );
	event->accept();
}

void PlayListView::dropFiles_() {
	emit this->fileDropped( this->droppingFiles_ );
}
