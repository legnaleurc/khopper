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

namespace Khopper {

	SongListView::SongListView( QWidget * parent ) : QTableView( parent ) {
		setAcceptDrops( true );
		// Set selection behavior
		setSelectionBehavior( SelectRows );
		setEditTriggers( DoubleClicked );
	}

	void SongListView::dragEnterEvent( QDragEnterEvent * event ) {
		if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
			event->acceptProposedAction();
		}
	}

	void SongListView::dragMoveEvent( QDragMoveEvent * event ) {
		if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
			event->acceptProposedAction();
		}
	}

	void SongListView::dropEvent( QDropEvent * event ) {
		if( event->mimeData()->hasUrls() ) {
			const QList< QUrl > & urlList = event->mimeData()->urls();
			for( QList< QUrl >::const_iterator it = urlList.begin(); it != urlList.end(); ++it ) {
				if( QFileInfo( it->toLocalFile() ).suffix() == "cue" ) {
					emit openFile( it->toLocalFile() );
					break;
				}
			}
		}
		event->acceptProposedAction();
	}

}
