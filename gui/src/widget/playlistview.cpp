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
#include "playlistmodel.hpp"

#include "khopper/playlist.hpp"

#include <QtCore/QFileInfo>
#include <QtCore/QSignalMapper>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtGui/QAction>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>

#include <algorithm>

using namespace khopper::widget;
using khopper::album::PlayList;
using khopper::utility::PlayListModel;

PlayListView::PlayListView( QWidget * parent ):
QTreeView( parent ),
contextMenu_( new QMenu( this ) ),
droppingFiles_(),
cmPos_() {
	// Set header
	this->header()->setMovable( true );

	// Set model
	QAction * delSong = new QAction( this );
	delSong->setShortcut( QKeySequence::Delete );
	this->addAction( delSong );
	connect( delSong, SIGNAL( triggered() ), this, SLOT( removeHelper_() ) );

	// Set context menu
	QMenu * codecs = new QMenu( tr( "Change Text Codec" ), this );

	QSignalMapper * sm = new QSignalMapper( this );

	// generate codec menu
	QList< int > mibs( QTextCodec::availableMibs() );
	std::for_each( mibs.begin(), mibs.end(), [&]( int mib ) {
		QAction * codec = new QAction( QTextCodec::codecForMib( mib )->name(), this );
		codecs->addAction( codec );
		QObject::connect( codec, SIGNAL( triggered() ), sm, SLOT( map() ) );
		sm->setMapping( codec, mib );
	} );

	connect( sm, SIGNAL( mapped( int ) ), this, SLOT( changeTextCodec_( int ) ) );

	this->contextMenu_->addMenu( codecs );

	this->contextMenu_->addSeparator();

	QAction * properties = new QAction( tr( "Properties" ), this );
	connect( properties, SIGNAL( triggered() ), this, SLOT( propertyHelper_() ) );
	this->contextMenu_->addAction( properties );

	this->contextMenu_->addSeparator();

	QAction * convert = new QAction( tr( "Convert" ), this );
	convert->setShortcut( Qt::CTRL + Qt::Key_Return );
	connect( convert, SIGNAL( triggered() ), this, SIGNAL( requireConvert() ) );
	this->addAction( convert );
	this->contextMenu_->addAction( convert );
}

void PlayListView::propertyHelper_() {
	emit this->requireProperty( this->indexAt( this->cmPos_ ) );
}

void PlayListView::changeTextCodec_( int mib ) {
	QTextCodec * codec = QTextCodec::codecForMib( mib );
	QModelIndexList selected = this->selectionModel()->selectedRows();
	std::for_each( selected.begin(), selected.end(), [&]( const QModelIndex & index ) {
		static_cast< PlayListModel * >( this->model() )->changeTextCodec( index, codec );
	} );
}

void PlayListView::removeHelper_() {
	static_cast< PlayListModel * >( this->model() )->remove( this->selectionModel()->selectedRows() );
	this->selectionModel()->clear();
}

void PlayListView::contextMenuEvent( QContextMenuEvent * event ) {
	QModelIndex index( this->indexAt( event->pos() ) );
	if( index.isValid() ) {
		this->cmPos_ = event->pos();
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
	event->accept();
}

void PlayListView::dropFiles_() {
	emit this->fileDropped( this->droppingFiles_ );
}
