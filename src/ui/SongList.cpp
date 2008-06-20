#include "SongList.hpp"

namespace Khopper {
	
	SongList::SongList( QWidget * parent ) : QTableView( parent ) {
		setAcceptDrops( true );
	}
	
	void SongList::dragEnterEvent( QDragEnterEvent * event ) {
		if( event->mimeData()->hasFormat( "text/uri-list" ) ) {
			event->acceptProposedAction();
		}
	}
	
	void SongList::dropEvent( QDropEvent * event ) {
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
