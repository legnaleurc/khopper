#include "songlist.hpp"

#include <QtGui>

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
